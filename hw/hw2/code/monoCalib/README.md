# monoCalib

## Overview

monoCalib is a project that showcases camera calibration using CMake and OpenCV. This guide provides instructions on how to build and run the project.

## Environment

Before you begin, ensure that you have the following installed:

1. CMake (version 3.12 or higher)
2. C++ compiler (supporting C++11)
3. OpenCV library

## How to Run

```bash
cd monocalib
```

Create a Build Directory

```bash
mkdir build
cd build
```

Run CMake

```bash
cmake ..
```

Build the Project: This step will compile the source code and generate the executable.

```bash
make
```

Run the Program

```bash
./monoCalib
```

## Result

The calibration parameters are saved in `build/calibration_result.txt`.