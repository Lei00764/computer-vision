# 指定CMake最低版本要求
cmake_minimum_required(VERSION 3.22)

# 设置项目名称
project(OpenCVCrash)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)

# 查找OpenCV库
find_package(OpenCV)

# 包含OpenCV头文件路径
include_directories(${OpenCV_INCLUDE_DIRS})

# 创建可执行文件
add_executable(main src/main.cpp)

# 链接OpenCV库
target_link_libraries(main ${OpenCV_LIBS})