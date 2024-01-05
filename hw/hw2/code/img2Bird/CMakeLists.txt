cmake_minimum_required(VERSION 3.12)
project(Img2Bird)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories for OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(img2Bird img2Bird.cpp)

# Link against OpenCV
target_link_libraries(img2Bird ${OpenCV_LIBS})