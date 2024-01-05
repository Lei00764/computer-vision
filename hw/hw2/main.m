% 该程序演示如何利用相机的内参数来进行图像的畸变去除
% 张林，同济大学，2023年4月
% 相机的内参数已经通过前期标定步骤获得，并已存储在磁盘中为文件'cameraParams.mat'
% 导入相机参数数据
camParamsFile = load('cameraParams.mat');
camPrams = camParamsFile.cameraParams;

% 依次读入文件夹中的图像，同款相机拍摄的原始图像，带有明显畸变
imageDirPath = "original_images";
imageDir = dir(imageDirPath);
imageNum = length(imageDir) - 2;  % 去除.和..两个文件夹
oriImg = cell(imageNum, 1);
for i = 1:imageNum
    oriImg{i} = imread(fullfile(imageDirPath, imageDir(i+2).name));
end

% 依次对每张图像进行畸变去除
undistortedImage = cell(imageNum, 1);
for i = 1:imageNum
    undistortedImage{i} = undistortImage(oriImg{i}, camPrams);
end

% 保存去除畸变后的图像
for i = 1:imageNum
    imwrite(undistortedImage{i}, fullfile('undistorted_images', imageDir(i+2).name));
end