#ifndef POINTMAPPER_H
#define POINTMAPPER_H

#pragma once
#pragma warning(disable: 4251)

#include "CalculateCorner.h"
#include "BoardType.h"
#include "DistortionLevel.h"
//#include "../pch.h"

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class DLL_EXPORT PointMapper{
	// 第二步，构建真实坐标系与像素坐标系的映射关系
public:
	PointMapper();

	// 添加接口：允许用户通过前端更改
	void SetSquareSize(float squareSize);
	void SetBoardSize(cv::Size boardSize);
	void SetBoardType(BoardType boardType);
	void SetDistortionLevel(DistortionLevel level);

	float GetSquareSize() const;
	cv::Size GetBoardSize() const;
	BoardType GetBoardType() const;
	DistortionLevel GetDistortionLevel() const;

	// 为单张图像生成真实点
	std::vector<cv::Point3f> GenerateRealWorldPoints(); 
	std::vector<cv::Point3f> GetRealWorldPoints() const;
	
	// 判断单张图片的真实点和像素点数量是否相同
	bool MapSingleImagePoint(std::vector<cv::Point2f>& imagePoints, std::vector<cv::Point3f>& worldPoints);

private:
	//CalculateCorner CalculateCorner;
	BoardType board_type;
	float SquareSize;
	cv::Size BoardSize;
	DistortionLevel distortionLevel;
	std::vector<cv::Point3f> RealWorldPoints;
};

#endif // !POINTMAPPER_H
