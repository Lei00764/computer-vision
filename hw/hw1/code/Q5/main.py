"""
Author: Xiang Lei
Created: 2023-10-16 11:20:18
Purpose: DoG and SIFT
"""

import os
import cv2
import numpy as np

"""
Reference: https://blog.csdn.net/qq_40374812/article/details/117079134
1. initilize a SIFT constructor
2. get keypoints and descriptors
3. get good matches
4. draw matches
5. get homography matrix
6. get homograyphy image
"""

root_path = "/Users/lei/Desktop/CV/hw/hw1/code/Q5"
save_dir_path = "/Users/lei/Desktop/CV/hw/hw1/code/Q5/src/result"
os.makedirs(save_dir_path, exist_ok=True)


# def get_DoG(image_path, sigma1=1.0, sigma2=2.0):
#     """

#     Args:
#         image_path (_type_): _description_
#         sigma1 (float, optional): _description_. Defaults to 1.0.
#         sigma2 (float, optional): _description_. Defaults to 2.0.

#     Returns:
#         _type_: _description_
#     """
#     image = cv2.imread(image_path, cv2.IMREAD_COLOR)

#     guassian1 = cv2.GaussianBlur(image, (0, 0), sigma1)
#     guassian2 = cv2.GaussianBlur(image, (0, 0), sigma2)

#     DoG = guassian1 - guassian2

#     save_name = os.path.splitext(os.path.basename(image_path))[0]
#     cv2.imwrite(f"{save_dir_path}/{save_name}_DoG.png", DoG)

#     return DoG


def get_keypoints_and_descriptors(image_path):
    """use SIFT to get keypoints and descriptors

    Args:
        image (_type_): _description_

    Returns:
        _type_: _description_
    """
    image = cv2.imread(image_path, cv2.IMREAD_COLOR)
    sift = cv2.SIFT_create()  # object

    keypoints, descriptors = sift.detectAndCompute(
        image, None)  # len(keypoints) == len(descriptors)
    kp_image = cv2.drawKeypoints(image, keypoints, None)

    save_name = os.path.splitext(os.path.basename(image_path))[0]
    cv2.imwrite(f"{save_dir_path}/{save_name}_keypoints.png", kp_image)

    return keypoints, descriptors


def get_good_matches(transformed_descriptors, fixed_descriptors):
    """get good matches

    Args:
        transformed_descriptors (_type_): _description_
        fixed_descriptors (_type_): _description_

    Returns:
        _type_: _description_
    """
    matcher = cv2.BFMatcher()
    matches = matcher.knnMatch(
        transformed_descriptors, fixed_descriptors, k=2)  # k=2: get 2 nearest neighbors for each descriptor
    # <DMatch 0x7f117af995f0>, <DMatch 0x7f117af99610>

    # filter
    good_matches = []
    for m, n in matches:  # m: the nearsest; n: the second nearsest
        if m.distance < 0.75 * n.distance:
            good_matches.append(m)

    return good_matches


def draw_matches(transformed_image_path, fixed_image_path, transformed_keypoints, fixed_keypoints, good_matches):
    """draw matches

    Args:
        transformed_image_path (_type_): _description_
        fixed_image_path (_type_): _description_
        transformed_keypoints (_type_): _description_
        fixed_keypoints (_type_): _description_
        good_matches (_type_): _description_
    """
    transformed_image = cv2.imread(transformed_image_path, cv2.IMREAD_COLOR)
    fixed_image = cv2.imread(fixed_image_path, cv2.IMREAD_COLOR)

    matches = good_matches[:100]

    # flags: how to draw the matches
    result = cv2.drawMatches(
        transformed_image, transformed_keypoints, fixed_image, fixed_keypoints, matches, None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

    save_name = os.path.splitext(os.path.basename(transformed_image_path))[0]
    cv2.imwrite(f"{save_dir_path}/{save_name}_matches.png", result)


def get_homography_matrix(transformed_keypoints, fixed_keypoints, good_matches):
    """get homography matrix

    Args:
        transformed_keypoints (_type_): _description_
        fixed_keypoints (_type_): _description_
        good_matches (_type_): _description_

    Returns:
        _type_: _description_
    """
    points1 = np.zeros((len(good_matches), 2), dtype=np.float32)
    points2 = np.zeros((len(good_matches), 2), dtype=np.float32)

    for i, match in enumerate(good_matches):
        points1[i, :] = transformed_keypoints[match.queryIdx].pt
        points2[i, :] = fixed_keypoints[match.trainIdx].pt

    # find homography
    homography_matrix, _ = cv2.findHomography(
        points1, points2, cv2.RANSAC)

    return homography_matrix


def get_homograyphy_image(transformed_image_path, fixed_image_path, homography_matrix):
    """get homograyphy image

    Args:
        transformed_image_path (_type_): _description_
        fixed_image_path (_type_): _description_
        homography_matrix (_type_): _description_
    """
    # warp source image to destination based on homography
    transformed_image = cv2.imread(transformed_image_path, cv2.IMREAD_COLOR)
    fixed_image = cv2.imread(fixed_image_path, cv2.IMREAD_COLOR)

    height, width, _ = fixed_image.shape
    image1_warped = cv2.warpPerspective(
        transformed_image, homography_matrix, (width, height))

    fixed_image[image1_warped > 0] = 0
    result = fixed_image + image1_warped

    save_name = os.path.splitext(os.path.basename(transformed_image_path))[0]
    cv2.imwrite(f"{save_dir_path}/{save_name}_trans_homograyphy.png", result)


def main():
    transformed_image_path = os.path.join(root_path, "src/sse1.bmp")
    fixed_image_path = os.path.join(root_path, "src/sse2.bmp")

    # step1: initilize a SIFT constructor
    # step2: get keypoints and descriptors
    transformed_keypoints, transformed_descriptors = get_keypoints_and_descriptors(
        transformed_image_path)
    fixed_keypoints, fixed_descriptors = get_keypoints_and_descriptors(
        fixed_image_path)

    # step3: get good matches
    good_matches = get_good_matches(
        transformed_descriptors, fixed_descriptors)

    # step4: draw matches
    draw_matches(transformed_image_path, fixed_image_path,
                 transformed_keypoints, fixed_keypoints, good_matches)

    # step5: get homography matrix
    homography_matrix = get_homography_matrix(
        transformed_keypoints, fixed_keypoints, good_matches)

    # step6: get homograyphy image
    get_homograyphy_image(transformed_image_path,
                          fixed_image_path, homography_matrix)


if __name__ == "__main__":
    main()
