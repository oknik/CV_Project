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
	// ��������������������ϵ����������ϵ����������б궨
public:
	CameraCalibrate();

	// ��Զ�����Ƭ����������궨��Ϊ
	// ��֧�־����������ѡ��
	void CalibrateStandardCamera(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		cv::Size imageSize,
		CalibrationOptions option
	);

	// �����������궨
	// ��֧��SkewУ���Ͳ����Ż�
	void CalibrateFishEyeCamera(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		cv::Size imageSize,
		CalibrationOptions option
	);

	// �����������
	void ExportCameraParameters(std::string outputFile, cv::Size imageSize) const;

	cv::Mat GetCameraMatrix() const;
	cv::Mat GetDistCoeffs() const;
	std::vector<cv::Mat> GetRotationVectors() const;
	std::vector<cv::Mat> GetTranslationVectors() const;

private:
	cv::Mat CameraMatrix; // ����ڲξ���
	cv::Mat DistCoeffs; // �������
	std::vector<cv::Mat> RotationVectors; // ��ת����
	std::vector<cv::Mat> TranslationVectors; // ƽ�Ʊ���
};

#endif // !CAMERACALIBRATION_H

