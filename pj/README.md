# Interaction between a mobile device and a ROS host

2053932 雷翔 2050633 卢嘉霖 2050664 黄晨冉

Cousre project for the course "Computer Vison" in 2023 Fall, Tongji University.

Instructor: [Lin Zhang](https://scholar.google.com.tw/citations?user=8VOk_S4AAAAJ&hl=zh-CN&oi=ao)

Tutor: [Zhong Wang](https://scholar.google.com.tw/citations?hl=zh-CN&user=rrkp_usAAAAJ)

Course web: <https://cslinzhang.gitee.io/home/CV/CV.htm>

Project web: <https://cslinzhang.gitee.io/home/CV/Projects/mobile-device-ROS.htm>

### File Structure

```bash
code
├── catkin_ws        // For cartographer_ros
├── catkin_ws2       // For A1M8
├── control          // For TCP 
└── iviz-CV-project  // For iviz
```

#### Mobile(iviz)

The entire iviz repository consists of multiple folders. The `iviz` folder is the main Unity project, while other folders starting with 'iviz_' contain a series of utility libraries that can be used in iviz projects (see the README.md in the root directory of the project for more details). 

Inside the `iviz` folder, the directory structure is as follows:

```Plain
.
├── Assets
│   ├── ARKitSettings.asset
│   ├── Application
│   ├── Core
│   ├── Dependencies
│   ├── Displays
│   ├── ImageDecoders
│   ├── Joystick Pack
│   ├── MarkerDetection
│   ├── Prefabs
│   ├── ROS
│   ├── Resources
│   ├── Scenes
│   ├── TF
│   ├── TextMesh Pro
│   ├── VNC
│   ├── XR
│   ├── XRI
│   ├── PauseButton.cs
│   ├── Switch2DButton.cs
│   ├── Switch3DButton.cs
│   └── TcpManager.cs
├── Library
├── Logs
├── Packages
├── ProjectSettings
├── README.md
└── UserSettings
```

This is a classical Unity project directory structure. All C# scripts are in `Assets` folder.

#### PC(ROS)

```bash
.
├── abseil-cpp
│   ├── ABSEIL_ISSUE_TEMPLATE.md
│   ├── AUTHORS
│   ├── BUILD.bazel
│   ├── CMake
│   ├── CMakeLists.txt
│   ├── CONTRIBUTING.md
│   ├── FAQ.md
│   ├── LICENSE
│   ├── README.md
│   ├── UPGRADES.md
│   ├── WORKSPACE
│   ├── absl
│   ├── build
│   ├── ci
│   ├── conanfile.py
│   └── create_lts.py
├── devel
│   ├── _setup_util.py
│   ├── cmake.lock
│   ├── env.sh
│   ├── lib
│   ├── local_setup.bash
│   ├── local_setup.sh
│   ├── local_setup.zsh
│   ├── setup.bash
│   ├── setup.sh
│   └── setup.zsh
└── src
    ├── CMakeLists.txt -> /opt/ros/noetic/share/catkin/cmake/toplevel.cmake
    ├── cartographer
    └── cartographer_ros
```

## Environment Configuration

### Mobile(iviz)

To run iviz, you need the following:

- For mobile: iOS (> 11.0) or Android (> 7.0)
- For the standalone package: Windows, Linux, or macOS
- For running from the editor: Unity 2021.3 (make sure to select the scene at 'Scenes/UI AR')

The Unity project has no external dependencies (all required libraries are included), so installing it is just a matter of cloning the repository and launching Unity.

### PC(ROS)

![截屏2024-01-05 11.04.59](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/%E6%88%AA%E5%B1%8F2024-01-05%2011.04.59.png)

![截屏2024-01-05 11.06.06](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/%E6%88%AA%E5%B1%8F2024-01-05%2011.06.06.png)

配置流程请见：https://google-cartographer-ros.readthedocs.io/en/latest/compilation.html

## Running Guide

### Mobile(iviz)

After launching iviz, there are two main configurations to which you need to pay attention.

- Click the URL(`localhost:11311`  here) on the control panel. Enter the IP address and port corresponding to the ROS process running on your host machine in the Master URI input field.

![截屏2024-01-05 10.32.33](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/%E6%88%AA%E5%B1%8F2024-01-05%2010.32.33.png)

- Enter the IP address and port of the TCP connection. This connection is for a hyper control on ROS process and commands between host and client. Usually the host process should be running on the same machine as the ROS process, thus TCP IP address is the same as the input address just now. TCP port is set in the socket of host process. You can check its script to obtain the port value.

### PC(ROS)

#### Play 2D/3D bag

2D bag

```bash
cd /home/parallels/catkin_ws

source install_isolated/setup.bash

roslaunch cartographer_ros demo_backpack_2d.launch bag_filename:=${HOME}/Downloads/cartographer_paper_deutsches_museum.bag  # start cartographer_ros node
```

3D bag

```bash
cd /home/parallels/catkin_ws

source install_isolated/setup.bash

roslaunch cartographer_ros demo_backpack_3d.launch bag_filename:=${HOME}/Downloads/b3-2016-04-05-14-14-00.bag
```

#### Real-time mapping

To effectively employ the A1M8 LiDAR for real-time mapping, it's necessary to compile the `catkin_ws2` workspace for radar scanning. Following this, you should proceed to initiate the RPLiDAR driver with the subsequent commands:

```bash
cd /home/parallels/catkin_ws2

source devel/setup.bash

roslaunch rplidar_ros view_rplidar.launch
```

Additionally, modifications are required for the Cartographer. Detailed instructions for these modifications can be found at this [blog post](https://blog.csdn.net/BIT_HXZ/article/details/122272239).

Once all modifications are complete, initiate the Cartographer node to receive radar scanning data with these commands:

```bash
cd /home/parallels/catkin_ws

source install_isolated/setup.bash

roslaunch cartographer_ros demo_revo_lds.launch
```

## Acknowledgments

The external code libraries used in this project include [cartographer_ros](https://github.com/cartographer-project/cartographer_ros). Thanks all the software developers for their selfless dedication to the open-source community!

https://blog.csdn.net/shsh0810/article/details/122360539

https://blog.csdn.net/BIT_HXZ/article/details/122272239

https://www.slamtec.com/cn/Support#rplidar-a-series

https://bucket-download.slamtec.com/d02e3f676b21b52eee3254e6566d97713d91ca98/LM108_SLAMTEC_rplidarkit_usermanual_A1M8_v2.1_cn.pdf

https://google-cartographer-ros.readthedocs.io/en/latest/compilation.html
