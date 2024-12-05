#ifndef IMAGEUNDISTORT_H
#define IMAGEUNDISTORT_H

#pragma once
#pragma warning(disable: 4251)
//#include "../pch.h"

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#include <opencv2/opencv.hpp>
#include <string>

class DLL_EXPORT ImageUndistort{
	// 第四步，根据相机参数对照片去畸变
public:
	ImageUndistort(cv::Mat cameraMatrix, cv::Mat distCoeffs);

	// 矫正图片
	void Undistort(cv::Mat distortedImage, bool isFishEye);
	cv::Mat GetUndistortImage() const;

	// 展示重投影的功能
	void DrawReprojectedImage(
		std::vector<cv::Point3f> worldPoints,
		std::vector<cv::Point2f> imagePoints,
		std::vector<cv::Mat> rotationVectors,
		std::vector<cv::Mat> translationVectors,
		cv::Size imageSize
	);
	cv::Mat GetReprojectedImage() const;

	// 计算重投影误差
	double ComputeReprojectionError(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		std::vector<cv::Mat> rotationVectors,
		std::vector<cv::Mat> translationVectors
	);

	// 保存去畸变图片
	void SaveUndistortedImage(std::string outputPath);

private:
	cv::Mat CameraMatrix;
	cv::Mat DistCoeffs;
	cv::Mat UndistortedImage;
	cv::Mat ReprojectedImage;
};

#endif // !IMAGEUNDISTORT

