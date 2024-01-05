"""
Author: Xiang Lei
Created: 2023-10-16 10:04:34
Purpose: Fit a straight 2D line using RANSAC
"""
import os
import random
import numpy as np
import matplotlib.pyplot as plt

"""
Reference: https://zhuanlan.zhihu.com/p/62238520
1. Select the minimum dataset from which a model can be estimated (for linear fitting, it's two points; for calculating the Homography matrix, it's four points).
2. Use this dataset to calculate the data model.
3. Plug all the data into this model and calculate the number of "inliers" (accumulate data that fits the model within a certain range of error for the current iteration).
4. Compare the number of "inliers" of the current model with the best model obtained previously, and record the model parameters and "inlier" count with the highest number.
5. Repeat steps 1-4 until the iteration ends or the current model is good enough (i.e., the "inlier" count is greater than a certain threshold).
"""

root_path = "/Users/lei/Desktop/CV/hw/hw1/code/Q4"
save_dir_path = "/Users/lei/Desktop/CV/hw/hw1/code/Q4/src/result"
os.makedirs(save_dir_path, exist_ok=True)


def ransac_linear(data, threshold, iterations, min_nums=3):
    """_summary_

    Args:
        data (_type_): sample points
        threshold (_type_): the threshold
        iterations (_type_): the number of iterations
        min_nums (_type_): the minimum number of points to fit the model
    """
    data_size = len(data)
    best_k = 0
    best_b = 0
    best_inliners_num = 0

    i = 0
    while i < iterations:
        # step1: select two points
        sample_index = random.sample(range(data_size), 2)
        # print(sample_index)

        x1, y1 = data[sample_index[0]]
        x2, y2 = data[sample_index[1]]
        # step2: calculate the data model
        k = (y2 - y1) / (x2 - x1)
        b = y1 - k * x1

        # step3: calculate the number of "inliers"
        inliers_num = 0
        for x, y in data:
            if abs(y - (k * x + b)) < threshold:
                inliers_num += 1

        # step4: compare
        if inliers_num > best_inliners_num:
            best_k = k
            best_b = b
            best_inliners_num = inliers_num

        if inliers_num > min_nums:
            i += 1

    return best_k, best_b


def main():
    sample_points = [(-2, 0), (0, 0.9), (2, 2.0), (3, 6.5), (4, 2.9), (5, 8.8), (6, 3.95), (8, 5.03), (10, 5.97),
                     (12, 7.1), (13, 1.2), (14, 8.2), (16, 8.5), (18, 10.1)]

    best_k, best_b = ransac_linear(sample_points, 0.5, 10000, 4)

    # plot
    plt.scatter([x[0] for x in sample_points], [x[1] for x in sample_points])
    plt.plot([-2, 20], [best_k * -2 + best_b,
             best_k * 20 + best_b], color='red')

    # setting
    plt.text(12, 4, "y = %.2f * x + %.2f" % (best_k, best_b), color='red')
    plt.title("RANSAC Linear Fitting")
    plt.grid()
    plt.xlim(-2, 18)
    plt.ylim(0, 12)
    plt.xticks(np.arange(-2, 20, 2))
    plt.yticks(np.arange(0, 14, 2))

    plt.show()

    # save
    # save_path = os.path.join(save_dir_path, "output.png")
    # plt.savefig(save_path)


if __name__ == "__main__":
    main()
