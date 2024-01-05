#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/stat.h> // For mkdir function

#include <opencv2/opencv.hpp>

// TARGET: ORB

std::string root_path = "/Users/lei/Desktop/CV/hw/hw1/code/Q6/";
// important: please comfile you have created the following folders
std::string save_dir_path = root_path + "images/result/";

void ORB(cv::Mat transformed_image, cv::Mat fixed_image)
{
    // step1: initilize a ORB constructor
    cv::Ptr<cv::ORB> orb = cv::ORB::create();

    // step2: get keypoints and descriptors
    std::vector<cv::KeyPoint> transformed_keypoints, fixed_keypoints;
    cv::Mat transformed_descriptors, fixed_descriptors;
    orb->detectAndCompute(transformed_image, cv::Mat(), transformed_keypoints, transformed_descriptors);
    orb->detectAndCompute(fixed_image, cv::Mat(), fixed_keypoints, fixed_descriptors);

    // step3: get good matches
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<cv::DMatch> matches;
    matcher.match(transformed_descriptors, fixed_descriptors, matches);

    // filter
    // get min and max distance
    // all descriptors in matches are sorted
    auto min_max = std::minmax_element(matches.begin(), matches.end(), [](const cv::DMatch &m1, const cv::DMatch &m2)
                                       { return m1.distance < m2.distance; });
    double min_dist = min_max.first->distance;
    double max_dist = min_max.second->distance;
    std::cout << "min_dist: " << min_dist << std::endl;
    // get good matches
    std::vector<cv::DMatch> good_matches;
    for (int i = 0; i < matches.size(); i++)
    {
        if (matches[i].distance <= std::max(2 * min_dist, 30.0)) // 30 is a threshold
        {
            good_matches.push_back(matches[i]);
        }
    }
    std::cout << "good_matches size: " << good_matches.size() << std::endl;

    // step4 : draw matches
    cv::Mat image_matches;
    cv::drawMatches(transformed_image, transformed_keypoints, fixed_image, fixed_keypoints, good_matches, image_matches);

    cv::imwrite(save_dir_path + "image_matches.png", image_matches);
    std::cout << "image_matches saved." << std::endl;

    // step5: get homography matrix
    std::vector<cv::Point2f>
        transformed_points, fixed_points;
    for (int i = 0; i < matches.size(); i++)
    {
        transformed_points.push_back(transformed_keypoints[matches[i].queryIdx].pt);
        fixed_points.push_back(fixed_keypoints[matches[i].trainIdx].pt);
    }
    cv::Mat H = cv::findHomography(transformed_points, fixed_points, cv::RANSAC);

    // step6: get homograyphy image
    cv::Mat transformed_image_transformed;
    cv::warpPerspective(transformed_image, transformed_image_transformed, H, fixed_image.size());

    cv::Mat homograyphy_image;
    // Set all corresponding pixels in transformed_image_transformed to 0
    fixed_image.setTo(cv::Scalar(0, 0, 0), (transformed_image_transformed > 0));
    // add the two images together
    homograyphy_image = fixed_image + transformed_image_transformed;

    cv::imwrite(save_dir_path + "homograyphy.png", homograyphy_image);
    std::cout << "homograyphy image saved." << std::endl;
}

int main()
{
    std::string transformed_path = root_path + "images/sse1.bmp";
    std::string fixed_path = root_path + "images/sse2.bmp";

    cv::Mat transformed_image = cv::imread(transformed_path, cv::IMREAD_COLOR);
    cv::Mat fixed_image = cv::imread(fixed_path, cv::IMREAD_COLOR);

    assert(!transformed_image.empty() && !fixed_image.empty());

    // a step to check whether the save_dir exists
    struct stat st;
    if (stat(save_dir_path.c_str(), &st) != 0)
    {
        if (mkdir(save_dir_path.c_str(), 0777) == 0)
        {
            std::cout << "Directory created: " << save_dir_path << std::endl;
        }
        else
        {
            std::cerr << "Error creating directory." << std::endl;
        }
    }

    // ORB
    ORB(transformed_image, fixed_image);

    return 0;
}