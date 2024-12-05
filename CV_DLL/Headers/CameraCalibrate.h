#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#pragma once
#pragma warning(disable: 4251)
#include "../pch.h"

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


#include "CalculateCorner.h"
#include "PointMapper.h"
#include "../Enums/CalibrationOptions.cpp"


#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class DLL_EXPORT CameraCalibrate{
	// 第三步，根据世界坐标系与像素坐标系，对相机进行标定
public:
	CameraCalibrate();

	// 针对多张照片，进行相机标定行为
	// 仅支持径向和切向方向选项
	void CalibrateStandardCamera(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		cv::Size imageSize,
		CalibrationOptions option
	);

	// 针对鱼眼相机标定
	// 仅支持Skew校正和参数优化
	void CalibrateFishEyeCamera(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		cv::Size imageSize,
		CalibrationOptions option
	);

	// 导出相机参数
	void ExportCameraParameters(std::string outputFile, cv::Size imageSize) const;

	cv::Mat GetCameraMatrix() const;
	cv::Mat GetDistCoeffs() const;
	std::vector<cv::Mat> GetRotationVectors() const;
	std::vector<cv::Mat> GetTranslationVectors() const;

private:
	cv::Mat CameraMatrix; // 相机内参矩阵
	cv::Mat DistCoeffs; // 畸变参数
	std::vector<cv::Mat> RotationVectors; // 旋转变量
	std::vector<cv::Mat> TranslationVectors; // 平移变量
};

#endif // !CAMERACALIBRATION_H

