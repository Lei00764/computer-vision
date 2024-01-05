#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

// define the size of the bird view image
const int BIRDS_EYE_WIDTH = 1000;
const int BIRDS_EYE_HEIGHT = 1000;

/**
 * @brief get the undistorted image object
 *
 * @param original_image
 * @param intrinsic_matrix
 * @param distortion_coefficients
 */
void get_undistorted_image(cv::Mat original_image, cv::Mat intrinsic_matrix,
                           cv::Mat distortion_coefficients) {
  // undistort the image
  cv::Mat undistorted_image;
  cv::undistort(original_image, undistorted_image, intrinsic_matrix,
                distortion_coefficients, intrinsic_matrix);

  // save the undistorted image
  cv::imwrite("undistorted_image.png", undistorted_image);

  std::cout << "Save undistorted image successfully." << std::endl;
}

/**
 * @brief generate bird view image
 *
 * @param undistorted_image
 */
void generate_brid_view1(cv::Mat undistorted_image) {
  // undistorted_image -> brid_view_image
  cv::Mat brid_view_image =
      cv::Mat::zeros(BIRDS_EYE_HEIGHT, BIRDS_EYE_WIDTH, CV_8UC3);

  // get feature points in chessboard
  std::vector<cv::Point2f> image_points;

  cv::Mat undistorted_image_with_feature_points = undistorted_image.clone();

  cv::findChessboardCorners(undistorted_image_with_feature_points,
                            cv::Size(9, 6), image_points);

  // draw the feature points
  cv::drawChessboardCorners(undistorted_image_with_feature_points,
                            cv::Size(9, 6), image_points, true);

  // save the image with feature points
  cv::imwrite("image_with_feature_points.png",
              undistorted_image_with_feature_points);

  std::cout << "Save image with feature points successfully." << std::endl;

  // define the four points of the brid view image
  std::vector<cv::Point2f> brid_view_points;
  brid_view_points.push_back(cv::Point2f(200, 200));
  brid_view_points.push_back(cv::Point2f(440, 200));
  brid_view_points.push_back(cv::Point2f(440, 350));
  brid_view_points.push_back(cv::Point2f(200, 350));

  // define the four points of the undistorted image, use the feature points
  std::vector<cv::Point2f> undistorted_image_points;
  undistorted_image_points.push_back(image_points[0]);   // left top
  undistorted_image_points.push_back(image_points[8]);   // right top
  undistorted_image_points.push_back(image_points[53]);  // right bottom
  undistorted_image_points.push_back(image_points[45]);  // left bottom

  // get the perspective transform matrix
  cv::Mat H =
      cv::getPerspectiveTransform(undistorted_image_points, brid_view_points);

  // get the bird view image
  cv::warpPerspective(undistorted_image, brid_view_image, H,
                      cv::Size(BIRDS_EYE_WIDTH, BIRDS_EYE_HEIGHT));

  // save the bird view image
  cv::imwrite("brid_view_image_method1.png", brid_view_image);

  std::cout << "Save brid view image successfully." << std::endl;
}

/**
 * @brief get the perspective transform matrix
 * @param image_path1
 * @param image_path2
 * @return
 */
cv::Mat get_perspective_matrix(std::string image_path1,
                               std::string image_path2) {
  cv::Mat image1 = cv::imread(image_path1);
  cv::Mat image2 = cv::imread(image_path2);

  // get feature points in chessboard
  std::vector<cv::Point2f> image_points1;
  std::vector<cv::Point2f> image_points2;

  cv::Mat undistorted_image_with_feature_points1 = image1.clone();
  cv::Mat undistorted_image_with_feature_points2 = image2.clone();

  cv::findChessboardCorners(undistorted_image_with_feature_points1,
                            cv::Size(9, 6), image_points1);
  cv::findChessboardCorners(undistorted_image_with_feature_points2,
                            cv::Size(9, 6), image_points2);
  // 点的数量
  std::cout << "image_points1.size() = " << image_points1.size() << std::endl;
  std::cout << "image_points2.size() = " << image_points2.size() << std::endl;

  // draw the feature points
  cv::drawChessboardCorners(undistorted_image_with_feature_points1,
                            cv::Size(9, 6), image_points1, true);
  cv::drawChessboardCorners(undistorted_image_with_feature_points2,
                            cv::Size(9, 6), image_points2, true);

  // save the image with feature points
  cv::imwrite("image_with_feature_points1.png",
              undistorted_image_with_feature_points1);
  cv::imwrite("image_with_feature_points2.png",
              undistorted_image_with_feature_points2);

  std::cout << "Save image with feature points successfully." << std::endl;

  std::vector<cv::Point2f> selected_points1 = {
      image_points1[0], image_points1[8], image_points1[45], image_points1[53]};
  std::vector<cv::Point2f> selected_points2 = {
      image_points2[0], image_points2[8], image_points2[45], image_points2[53]};

  // get the perspective transform matrix
  cv::Mat H = cv::getPerspectiveTransform(selected_points1, selected_points2);

  return H;
}

void generate_brid_view2(cv::Mat undistorted_image) {
  // undistorted_image -> brid_view_image
  cv::Mat brid_view_image =
      cv::Mat::zeros(BIRDS_EYE_HEIGHT, BIRDS_EYE_WIDTH, CV_8UC3);

  // get feature points in chessboard
  std::vector<cv::Point2f> image_points;

  cv::Mat undistorted_image_with_feature_points = undistorted_image.clone();

  cv::findChessboardCorners(undistorted_image_with_feature_points,
                            cv::Size(9, 6), image_points);

  // draw the feature points
  cv::drawChessboardCorners(undistorted_image_with_feature_points,
                            cv::Size(9, 6), image_points, true);

  // save the image with feature points
  cv::imwrite("image_with_feature_points.png",
              undistorted_image_with_feature_points);

  std::cout << "Save image with feature points successfully." << std::endl;

  cv::Mat H = get_perspective_matrix(
      "/Users/lei/Desktop/CV/hw/hw2/code/img2Bird/src.png",
      "/Users/lei/Desktop/CV/hw/hw2/code/img2Bird/dst.png");

  // get the bird view image
  cv::warpPerspective(undistorted_image, brid_view_image, H,
                      cv::Size(BIRDS_EYE_WIDTH, BIRDS_EYE_HEIGHT));

  // save the bird view image
  cv::imwrite("brid_view_image_method2.png", brid_view_image);

  std::cout << "Save brid view image successfully." << std::endl;
}

int main() {
  std::string image_path = "/Users/lei/Desktop/CV/hw/hw2/code/img2Bird/src.png";
  // std::string image_path =
  // "/Users/lei/Desktop/CV/hw/hw2/code/img2Bird/img.png";

  // load the image
  cv::Mat original_image = cv::imread(image_path);
  if (original_image.empty()) {
    std::cerr << "Failed to open image file " << image_path << std::endl;
    return -1;
  }

  // read camera parameters from xml file
  cv::FileStorage fs;
  fs.open("/Users/lei/Desktop/CV/hw/hw2/code/img2Bird/camParams.xml",
          cv::FileStorage::READ);
  if (!fs.isOpened()) {
    std::cerr << "Failed to open camParams.xml" << std::endl;
    return -1;
  }

  cv::Mat intrinsic_matrix, distortion_coefficients;
  fs["intrinsic_matrix"] >> intrinsic_matrix;
  fs["distortion_coefficients"] >> distortion_coefficients;
  fs.release();

  std::cout << "intrinsic_matrix: \n" << intrinsic_matrix << std::endl;
  std::cout << "distortion_coefficients: \n"
            << distortion_coefficients << std::endl;

  // undistort the image
  get_undistorted_image(original_image, intrinsic_matrix,
                        distortion_coefficients);

  // generate_brid_view1(original_image);  // for the first method
  generate_brid_view2(original_image);  // for the second method

  return 0;
}