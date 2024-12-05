#include "../pch.h"
#include "../Headers/CameraCalibrate.h"

CameraCalibrate::CameraCalibrate(){}

void CameraCalibrate::CalibrateStandardCamera(
	std::vector<std::vector<cv::Point3f>> worldPoints,
	std::vector<std::vector<cv::Point2f>> imagePoints,
	cv::Size imageSize,
	CalibrationOptions option
) {
	int flag = 0;
	
	// 径向畸变类型设置标定标志
	if (option.RadialDistortion == RadialDistortionType::TWO_COEFFICIENTS)
		flag |= cv::CALIB_FIX_K3;
	// 禁用切向畸变
	if (!option.UseTangentialDistortion)
		flag |= cv::CALIB_ZERO_TANGENT_DIST;

	double rms = cv::calibrateCamera(
		worldPoints, imagePoints, imageSize,
		this->CameraMatrix, this->DistCoeffs,
		this->RotationVectors, this->TranslationVectors, flag);

	std::cout << "Standard camera calibration completed with RMS error = " << rms << std::endl;
}

void CameraCalibrate::CalibrateFishEyeCamera(
	std::vector<std::vector<cv::Point3f>> worldPoints,
	std::vector<std::vector<cv::Point2f>> imagePoints,
	cv::Size imageSize,
	CalibrationOptions option
) {
	int flag = 0;

	// 是否优化初始参数
	if (option.OptimizeParameters)
		flag |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
	// 是否矫正斜度
	if (option.SkewEnabled)
		flag |= cv::fisheye::CALIB_FIX_SKEW;

	cv::Mat K = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat D = cv::Mat::zeros(4, 1, CV_64F);
	std::vector<cv::Mat> rvecs, tvecs;

	double rms = cv::fisheye::calibrate(
		worldPoints, imagePoints, imageSize,
		K, D, rvecs, tvecs, flag
	);

	this->CameraMatrix = K;
	this->DistCoeffs = D;
	this->RotationVectors = rvecs;
	this->TranslationVectors = tvecs;

	std::cout << "Fisheye camera calibration completed with RMS error = " << rms << std::endl;
}

void CameraCalibrate::ExportCameraParameters(std::string outputFile, cv::Size imageSize) const {
	if (this->CameraMatrix.empty() || this->DistCoeffs.empty())
		throw std::runtime_error("camera parameter not available, please perform calibration first");

	std::ofstream File(outputFile);

	if (!File.is_open())
		throw std::runtime_error("unable to open file for writing: " + outputFile);

	// 计算焦距
	double fx = this->CameraMatrix.at<double>(0, 0); 
	double fy = this->CameraMatrix.at<double>(1, 1);
	File << "Focal Length : [ " << fx << " , " << fy << " ] \n";

	// 计算主点
	double cx = this->CameraMatrix.at<double>(0, 2);
	double cy = this->CameraMatrix.at<double>(1, 2);
	File << "Principle point : [ " << cx << " , " << cy << " ] \n";

	// 计算图片大小
	File << "image size : [ " << imageSize.width << " , " << imageSize.height << " ]\n";

	// 径向畸变
	File << "RadialDistortion: [";
	for (int i = 0; i < std::min(3, this->DistCoeffs.cols); i++) {
		File << this->DistCoeffs.at<double>(0, i);
		if (i < 2) File << ", ";
	}
	File << "]\n";

	// 向向畸变
	if (this->DistCoeffs.cols >= 5) {
		double p1 = this->DistCoeffs.at<double>(0, 2);
		double p2 = this->DistCoeffs.at<double>(0, 3);
		File << "TangentialDistortion: [" << p1 << ", " << p2 << "]\n";
	}
	else {
		File << "TangentialDistortion: [0, 0]\n";
	}

	// 倾斜参数
	double skew = this->CameraMatrix.at<double>(0, 1);
	File << "Skew: " << skew << "\n";

	// Camera Matrix (K)
	File << "CameraMatrix (K):\n";
	File << "[ " << this->CameraMatrix.at<double>(0, 0) << ", " << this->CameraMatrix.at<double>(0, 1) << ", " << this->CameraMatrix.at<double>(0, 2) << ";\n";
	File << "  " << this->CameraMatrix.at<double>(1, 0) << ", " << this->CameraMatrix.at<double>(1, 1) << ", " << this->CameraMatrix.at<double>(1, 2) << ";\n";
	File << "  " << this->CameraMatrix.at<double>(2, 0) << ", " << this->CameraMatrix.at<double>(2, 1) << ", " << this->CameraMatrix.at<double>(2, 2) << " ]\n";

	File.close();
	std::cout << "Camera parameters exported to: " << outputFile << std::endl;

}

cv::Mat CameraCalibrate::GetCameraMatrix() const {
	return this->CameraMatrix;
}

cv::Mat CameraCalibrate::GetDistCoeffs() const {
	return this->DistCoeffs;
}

std::vector<cv::Mat> CameraCalibrate::GetRotationVectors() const {
	return this->RotationVectors;
}

std::vector<cv::Mat> CameraCalibrate::GetTranslationVectors() const {
	return this->TranslationVectors;
}
