#ifndef CALCULATECORNER_H
#define CALCULATECORNER_H

#pragma once
#pragma warning(disable: 4251)

//#ifdef BUILD_DLL
//#define DLL_EXPORT __declspec(dllexport)
//#else
//#define DLL_EXPORT __declspec(dllimport)
//#endif
#define DLL_EXPORT __declspec(dllimport)
//#include "../pch.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

class DLL_EXPORT CalculateCorner{
	// 第一步，计算图片角点相关代码
public:
	CalculateCorner();
	~CalculateCorner() {}

	// 设置图片路径和标定板大小
	void AddImagePath(std::string imagePath);
	void RemoveImagePath(std::string imagePath);
	void ResetImagePathVector();
	void SetBoardSize(cv::Size boardSize);

	// 从摄像头捕获图像并保存到特定位置
	// cameraIndex = 0代表使用机器自带的摄像头, 图片保存路径没设默认值，由前端决定
	bool CaptureImageFromCamera(std::string savePath, int cameraIndex);

	//绘制xy轴
	void DrawAxisOnChessBoard(cv::Mat image, std::vector<cv::Point2f> corners);

	// 绘制特定的照片的角点
	// 计算角点值，区分是否为鱼眼相机
	// 如果找到了角点，会将图片呈现在屏幕上，关闭图片后会继续执行后续代码
	void CalculateSingleImageCorners(std::string imagePath, bool drawCorner, bool isFishEye); 
	// 绘制存储在ImagePathVector所有图片的角点
	void CalculateEveryImageCorners(bool drawCorner, bool isFishEye);

	// 获取所有的角点图
	std::vector<std::vector<cv::Point2f>> GetAllImageCorners();
	// 返回字典，允许根据文件路径去找对应的某张图的角点信息
	std::map<std::string, std::vector<cv::Point2f>> GetImageCorner() const;
	// 返回字典，允许根据上传的文件路径找到处理后的角点图
	std::map<std::string, cv::Mat> GetProcessedImageByPath() const;
	// 获取文件路径
	std::vector<std::string> GetAllFilePaths() const;

private:
	std::vector<std::string> ImagePathVector;
	cv::Size BoardSize;
	// 依据文件路径获取计算出的角点值
	std::map<std::string, std::vector<cv::Point2f>> FindCornersByPath;
	std::map<std::string, cv::Mat> FindProcessedImageByPath;
};

#endif // !CALCULATECORNER_H
