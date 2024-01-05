> > For submission, you **only** need to submit the following items to TA: 
> >
> > 1) the intrinsic parameters of your camera; 
> > 2) the original image of the wall (or other physical planes) taken by your camera; make sure that your name is painted or attached on the wall (or the plane); (maybe similar to following image I provide to you) 
> > 3) the generated bird’s-eye-view image of the wall (or other physical planes). 

Name: Xiang Lei (雷翔)

Student ID: 2053932

Instructor: [Lin Zhang](https://scholar.google.com.tw/citations?user=8VOk_S4AAAAJ&hl=zh-CN&oi=ao)

Tutor: [Zhong Wang](https://scholar.google.com.tw/citations?hl=zh-CN&user=rrkp_usAAAAJ)

Course web: <https://cslinzhang.gitee.io/home/CV/CV.htm>

If you have any questions or suggestions, please contact me via email at [2053932@tongji.edu.cn](mailto:2053932@tongji.edu.cn).

## Intrinsic parameters

The camera model and basic parameters used in this assignment:

**Camera Type:** Industrial Camera MV-SUA134GC

- **Model:** MV-SUA134GC
- **Optical Size:** 1/2"
- **Sensor Type:** CMOS
- **Resolution:** 1280x1024 pixels
- **Frame Rate:** 211 FPS (Frames Per Second)

Reference Code: [monoCalib](https://github.com/csLinZhang/CVBook/tree/main/chapter-10-imaging%20model%20and%20intrinsics%20calibration/03-monoCalib)

1. **Intrinsic Parameter Matrix:**

$$
K = 
\begin{bmatrix}
1743.7251 & 0 & 629.74518 \\
0 & 1756.26 & 434.37491 \\
0 & 0 & 1
\end{bmatrix}
$$

2. **Distortion Coefficients:**

$$
D(k_1, k_2, ρ_1, ρ_2, k_3) 

= [-0.48823568, 0.5023573, 0.0050766608, 0.0039613489, -1.4270054]
$$

## The bird's eye view 

The essence of a bird's eye view is to "reproject" the information from the image plane onto the ground plane. Therefore, the key step is to obtain the projection transformation relationship $H$ between these two planes.

## Result

### Method 1

This method involves using a single image. Four points are selected on a chessboard within the image. Then, a corresponding region is determined in the bird's eye view, and the homography matrix is calculated between these points in the image and their counterparts in the bird's eye view. And then, utilizing the computed homography matrix, a bird's eye view image is generated.

Original Image:

![img](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/img.png)

Bird's Eye View Image Generated After Distortion Removal:

![brid_view_image_method1](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/brid_view_image_method1.jpg)

### Method 2

This method uses two images taken from different perspectives: one from a top-down view and another from a frontal view. By capturing the same chessboard pattern from these viewpoints, it computes the homography matrix based on the corresponding points identified in both images. And then, utilizing the computed homography matrix, a bird's eye view image is generated.

Original Image:

![src](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/src.png)

Bird's Eye View Image Generated After Distortion Removal:

![brid_view_image_method2](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/brid_view_image_method2.jpg)

## Reference

https://github.com/csLinZhang/CVBook/tree/main/chapter-10-imaging%20model%20and%20intrinsics%20calibration/03-monoCalib

https://blog.csdn.net/qq_36622009/article/details/102539334


https://blog.csdn.net/sheziqiong/article/details/125541694