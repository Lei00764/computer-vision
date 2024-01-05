cmake_minimum_required(VERSION 3.12)
project(MonoCalibProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories for OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(monoCalib monoCalib.cpp myLM.cpp)

# Link against OpenCV
target_link_libraries(monoCalib ${OpenCV_LIBS})