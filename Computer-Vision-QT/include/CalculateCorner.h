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
	// ��һ��������ͼƬ�ǵ���ش���
public:
	CalculateCorner();
	~CalculateCorner() {}

	// ����ͼƬ·���ͱ궨���С
	void AddImagePath(std::string imagePath);
	void RemoveImagePath(std::string imagePath);
	void ResetImagePathVector();
	void SetBoardSize(cv::Size boardSize);

	// ������ͷ����ͼ�񲢱��浽�ض�λ��
	// cameraIndex = 0����ʹ�û����Դ�������ͷ, ͼƬ����·��û��Ĭ��ֵ����ǰ�˾���
	bool CaptureImageFromCamera(std::string savePath, int cameraIndex);

	//����xy��
	void DrawAxisOnChessBoard(cv::Mat image, std::vector<cv::Point2f> corners);

	// �����ض�����Ƭ�Ľǵ�
	// ����ǵ�ֵ�������Ƿ�Ϊ�������
	// ����ҵ��˽ǵ㣬�ὫͼƬ��������Ļ�ϣ��ر�ͼƬ������ִ�к�������
	void CalculateSingleImageCorners(std::string imagePath, bool drawCorner, bool isFishEye); 
	// ���ƴ洢��ImagePathVector����ͼƬ�Ľǵ�
	void CalculateEveryImageCorners(bool drawCorner, bool isFishEye);

	// ��ȡ���еĽǵ�ͼ
	std::vector<std::vector<cv::Point2f>> GetAllImageCorners();
	// �����ֵ䣬��������ļ�·��ȥ�Ҷ�Ӧ��ĳ��ͼ�Ľǵ���Ϣ
	std::map<std::string, std::vector<cv::Point2f>> GetImageCorner() const;
	// �����ֵ䣬��������ϴ����ļ�·���ҵ������Ľǵ�ͼ
	std::map<std::string, cv::Mat> GetProcessedImageByPath() const;
	// ��ȡ�ļ�·��
	std::vector<std::string> GetAllFilePaths() const;

private:
	std::vector<std::string> ImagePathVector;
	cv::Size BoardSize;
	// �����ļ�·����ȡ������Ľǵ�ֵ
	std::map<std::string, std::vector<cv::Point2f>> FindCornersByPath;
	std::map<std::string, cv::Mat> FindProcessedImageByPath;
};

#endif // !CALCULATECORNER_H
