"""
Author: Xiang Lei
Created: 2023-10-12 19:06:31
Purpose: Harris 角点检测算法实现
"""
import os
import argparse
import cv2
import time
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument("--window_size", type=int, default=5,
                    help="The size of the sliding window")
parser.add_argument("--k", type=float, default=0.05,
                    help="The free parameter in the equation")
parser.add_argument("--thresh", type=int, default=10000,
                    help="Threshold for corner detection")
parser.add_argument("--max_corners", type=int, default=1000,
                    help="Max number of corners to detect")
parser.add_argument("--nms_radius", type=int, default=10,
                    help="Radius for non-maximum suppression")
parser.add_argument("--debug", action="store_true", default=False,
                    help="Debug mode")
args = parser.parse_args()


def nms(corner_response, nms_radius=10):
    assert (nms_radius >= 0)

    height, width = corner_response.shape
    zeros = np.zeros_like(corner_response)
    max_mask = np.zeros_like(corner_response, dtype=bool)

    for y in range(nms_radius, height - nms_radius):
        for x in range(nms_radius, width - nms_radius):
            local_scores = corner_response[y - nms_radius:y +
                                           nms_radius + 1, x - nms_radius:x + nms_radius + 1]
            local_max = np.max(local_scores)

            if corner_response[y, x] == local_max:
                max_mask[y - nms_radius:y + nms_radius + 1, x -
                         nms_radius:x + nms_radius + 1] = (local_scores == local_max)

    return np.where(max_mask, corner_response, zeros)


def findCorners(image, window_size=5, k=0.05, thresh=10000, W=None, max_corners=100, nms_radius=10, debug=False):
    """
    Finds and returns list of corners and new image with corners drawn

    Args:
        images (_type_): _description_
        window_size (_type_): The size of the sliding window
        k (_type_): _description_
        W (_type_): _description_ Weight matrix
        thresh (_type_): _description_
    """
    height, width, _ = image.shape

    gray_image = cv2.cvtColor(
        image, cv2.COLOR_BGR2GRAY)  # convert to grayscale

    # get gradient in x and y direction
    Iy, Ix = np.gradient(gray_image)  # Ix = Iy = 2D array
    Iy = np.abs(Iy)
    Ix = np.abs(Ix)

    if debug:
        cv2.imwrite("./src/Ix.png", Ix)
        cv2.imwrite("./src/Iy.png", Iy)

    # get Ixx, Ixy, Iyy
    Ixx = Ix**2
    Ixy = Iy*Ix
    Iyy = Iy**2

    offset = window_size//2

    # create empty array to store corner response
    corner_response = np.zeros((height, width))

    print("Finding Corners...")
    # Loop through image and find our corners
    for y in range(offset, height-offset):
        for x in range(offset, width-offset):

            M11 = Ixx[y-offset:y+offset+1, x-offset:x+offset+1]
            M12 = Ixy[y-offset:y+offset+1, x-offset:x+offset+1]
            M21 = Ixy[y-offset:y+offset+1, x-offset:x+offset+1]
            M22 = Iyy[y-offset:y+offset+1, x-offset:x+offset+1]

            if W is not None:
                M11 = M11*W
                M12 = M12*W
                M21 = M21*W
                M22 = M22*W

            M = np.array([
                [M11.sum(), M12.sum()],
                [M21.sum(), M22.sum()]
            ])

            """
            det = np.linalg.det(M)  # complicated
            trace = np.trace(M)
            """
            # In fact, no need to compute lambda1 and lambda2 directly
            det = M[0, 0]*M[1, 1] - M[0, 1]*M[1, 0]
            trace = M[0, 0] + M[1, 1]

            r = det - k*(trace**2)

            corner_response[y, x] = r

    # nms
    corner_response_nms = nms(corner_response, nms_radius)

    print(corner_response_nms.sum(), corner_response.sum())

    # Find top corner candidates above a threshold
    corner_response_threshold = corner_response_nms > thresh
    print(corner_response)

    # Find the coordinates of the top xxx corner points
    top_corner_coordinates = np.argwhere(corner_response_threshold)
    top_corner_coordinates = top_corner_coordinates[0:max_corners]

    print(len(top_corner_coordinates))

    if len(top_corner_coordinates) < max_corners:
        print(
            f"Warning: Found fewer than {max_corners} corners ({len(top_corner_coordinates)} corners).")

    selected_corners_image = image.copy()

    for corner in top_corner_coordinates:
        y, x = corner
        cv2.circle(selected_corners_image, (x, y), 3, (0, 0, 255), -1)

    cv2.imwrite("./src/result.png", selected_corners_image)
    print("Done!")


def gaussian_matrix(size=5, sigma_x=1.0, sigma_y=1.0):
    """generates a 2D Gaussian weight matrix centered at the origin

    Args:
        size (int, optional): _description_. Defaults to 5.
        sigma_x (float, optional): _description_. Defaults to 1.0.
        sigma_y (float, optional): _description_. Defaults to 1.0.

    Returns:
        _type_: _description_
    """
    x, y = np.meshgrid(np.linspace(-size//2, size//2, size),
                       np.linspace(-size//2, size//2, size))
    weight_matrix = np.exp(-((x**2 / (2 * sigma_x**2)) +
                           (y**2 / (2 * sigma_y**2))))
    return weight_matrix


def main():
    image_path = "./src/h1xhV.jpg"
    image = cv2.imread(image_path, cv2.IMREAD_COLOR)

    # 2D Gaussian distribution
    W = gaussian_matrix(args.window_size)
    # print(W)

    findCorners(image, window_size=args.window_size,
                k=args.k, thresh=args.thresh, W=W, max_corners=args.max_corners, nms_radius=args.nms_radius, debug=args.debug)


if __name__ == "__main__":
    main()
