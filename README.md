# 2023-Computer-Vision

同济大学 2023 年《计算机视觉》课程资料

### Structure

```bash
.
├── CVBook
├── README.md
├── code
├── hw  # homework
├── pj  # project
├── ppt
└── ref
```

## Homework

### Homework 1

![截屏2023-11-22 14.00.58](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/%E6%88%AA%E5%B1%8F2023-11-22%2014.00.58.png)

For details, please refer to [hw/hw1/README.md](hw/hw1/README.md)

### Homework 2

![截屏2024-01-05 12.25.36](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/%E6%88%AA%E5%B1%8F2024-01-05%2012.25.36.png)

For details, please refer to [hw/hw2/README.md](hw/hw2/README.md)

### Homework 3

Easy to implement, no details.

## Project

### Requirements

In this project, your task is to develop a simple interactive app between a mobile device and a ROS host, based on Android or IOS. The app should realize the following functions:

- [x] 1. Start a SLAM application (Google’s cartographer 2D/3D) on the ROS master from the app. The program will play the data bag collected in advance, and then localize and map simultaneously.
- [x] 2. The app needs to receive messages sent by the SLAM program and display them (3/6-DoF poses and the built map (2D grid map or 3D point cloud)) on its UI.
- [x] 3. The app sends a command to early exit the SLAM program.

> **Caution:** In the above-mentioned tasks, 2D is the basic requirement. Encouraging students to achieve 3D display can result in extra bonus.

### What we do?

- [x] 1. Start a SLAM application (Google cartographer) in the ROS system.
- [x] 2. **Combined with hardware, self-purchased RPLIDAR A1M8 LiDAR to realize autonomous map building.**
- [x] 3. **The use of Sprint Lidar to generate a 3D point cloud map.**
- [x] 4. The cartographer accepts the radar data and sends it to the mobile app after processing.
- [x] 5. Real-time synchronization between the app and cartographer.
- [x] 6. Display of 2D grid map and **3D point cloud.**
- [x] 7. **Switch between 2D and 3D maps on mobile.**
- [x] 8. Exit SLAM program from the app.

For details, please refer to [pj/README.md](pj/README.md)

## Reference

CVBook: https://github.com/csLinZhang/CVBook