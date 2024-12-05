#include "../pch.h"
#include "../Headers/CalculateCorner.h"
//#include <windows.h>

CalculateCorner::CalculateCorner() : BoardSize(cv::Size(0, 0)) {
	this->ImagePathVector.clear();
	this->FindCornersByPath.clear();
	this->FindProcessedImageByPath.clear();
}

void CalculateCorner::SetBoardSize(cv::Size boardSize) {
	this->BoardSize = boardSize;
}

void CalculateCorner::AddImagePath(std::string imagePath) {
	this->ImagePathVector.push_back(imagePath);
}

void CalculateCorner::RemoveImagePath(std::string imagePath) {
	this->ImagePathVector.erase(
		std::remove(this->ImagePathVector.begin(), this->ImagePathVector.end(), imagePath),
		this->ImagePathVector.end()
	);
}

void CalculateCorner::ResetImagePathVector() {
	this->ImagePathVector.clear();
}

bool CalculateCorner::CaptureImageFromCamera(std::string savePath, int cameraIndex = 0) {
	cv::VideoCapture Camera(cameraIndex); // 打开摄像头
	if (!Camera.isOpened()) {
		std::cerr << "Error: unable to open camera" << std::endl;
		return false;
	}

	cv::Mat CapturedImage;
	std::cout << "Press SPACE to capture image or press ESC to exit" << std::endl;

	// 读取图像
	while (true) {
		Camera >> CapturedImage;
		if (CapturedImage.empty()) {
			std::cerr << "Error: captured frame is empty" << std::endl;
			return false;
			break;
		}
		cv::flip(CapturedImage, CapturedImage, 1);
		cv::imshow("Camera", CapturedImage);
		// 等待按键
		char key = static_cast<char>(cv::waitKey(30));

		if (key == 27) {
			std::cout << "Exiting capture image" << std::endl;
			break;
		}
		if (key == ' ') {
			if (cv::imwrite(savePath, CapturedImage)) {
				std::cout << "Image captured and saved to : " << savePath << std::endl;
				this->AddImagePath(savePath);
				break;
			}
			else {
				std::cerr << "Error: Failed to save the image" << std::endl;
				return false;
			}
			
		}
	}

	return true;
}

void CalculateCorner::DrawAxisOnChessBoard(cv::Mat image, std::vector<cv::Point2f> corners) {
	if (corners.empty()) {
		std::cerr << "No corners available for drawing axis" << std::endl;
		return;
	}

	// 确保图像是彩色的（避免绘制颜色无效）
	if (image.channels() == 1) {
		cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
	}

	// 确定原点
	cv::Point2f OriginPoint = corners[0];

	// 确定x方向
	if (corners.size() > 1) {
		cv::Point2f xAxis = corners[1];
		cv::arrowedLine(image, OriginPoint, xAxis, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);// 红色箭头
		cv::putText(image, "X-Axis", xAxis + cv::Point2f(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
	}

	// 确定y方向
	if (corners.size() > this->BoardSize.width) {
		cv::Point2f yAxis = corners[this->BoardSize.width]; // 第二行的第一个点
		cv::arrowedLine(image, OriginPoint, yAxis, cv::Scalar(0, 255, 0), 3, cv::LINE_AA); // 绿色箭头
		cv::putText(image, "Y-Axis", yAxis + cv::Point2f(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
	}

	// 绘制原点
	cv::circle(image, OriginPoint, 8, cv::Scalar(255, 0, 0), -1); // 蓝色圆点
	cv::putText(image, "Origin Point", OriginPoint + cv::Point2f(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);

}

void CalculateCorner::CalculateSingleImageCorners(std::string imagePath, bool drawCorner, bool isFishEye) {

	//wchar_t  buffer[MAX_PATH];
	//GetModuleFileName(NULL, buffer, MAX_PATH);
	//std::wstring ws(buffer);
	//std::string path(ws.begin(), ws.end());
	//std::cout << path << std::endl;


	if (imagePath.empty()) {
		std::cerr << "Image path is empty." << std::endl;
		return;
	}

	cv::Mat OriginalImage = cv::imread(imagePath);

	if (OriginalImage.empty()) {
		std::cerr << "Can not load image: " << imagePath << std::endl;
		return;
	}

	// 将原图转化为灰度图
	cv::Mat GrayImage;
	cv::cvtColor(OriginalImage, GrayImage, cv::COLOR_BGR2GRAY);

	std::vector<cv::Point2f> corners;
	bool FoundCorner = cv::findChessboardCorners(
		GrayImage, this->BoardSize, corners,
		cv::CALIB_CB_ADAPTIVE_THRESH |
		cv::CALIB_CB_NORMALIZE_IMAGE |
		(isFishEye ? 0 : cv::CALIB_CB_FAST_CHECK));
	if (FoundCorner) {
		// cv::Size(11, 11)：搜索窗口大小，平衡精度和速度。
		// cv::Size(-1, -1)：无死区，完整计算
		// 30, 0.001：迭代次数和精度阈值，控制优化终止条件
		cv::cornerSubPix(GrayImage, corners, cv::Size(11, 11), cv::Size(-1, -1),
			cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001));
		this->FindCornersByPath[imagePath] = corners;
		std::cout << "Corners is detected and set successfully" << std::endl;

		cv::drawChessboardCorners(OriginalImage, this->BoardSize, corners, FoundCorner);
		DrawAxisOnChessBoard(OriginalImage, corners);
		std::cout << "has drawn images with corners" << std::endl;
		this->FindProcessedImageByPath[imagePath] = OriginalImage;

		if (drawCorner) {
			cv::imshow("Corners with axis", OriginalImage);
			cv::waitKey(0);
		}
	}
	else {
		std::cerr << "Failed to detect corners" << std::endl;
		this->FindCornersByPath[imagePath] = std::vector<cv::Point2f>();
	}
}

void CalculateCorner::CalculateEveryImageCorners(bool drawCorner, bool isFishEye) {
	for (auto path : this->ImagePathVector) {
		CalculateSingleImageCorners(path, drawCorner, isFishEye);
	}
}

std::vector<std::vector<cv::Point2f>> CalculateCorner::GetAllImageCorners() {
	std::vector<std::vector<cv::Point2f>> AllImageCorners;
	if (this->FindCornersByPath.empty()) {
		std::cout << "no corners has been calculated" << std::endl;
		return std::vector<std::vector<cv::Point2f>>();
	}
	for (auto pair : this->FindCornersByPath) {
		AllImageCorners.push_back(pair.second);
	}
	return AllImageCorners;
}

std::map<std::string, std::vector<cv::Point2f>> CalculateCorner::GetImageCorner() const {
	return this->FindCornersByPath;
}

std::vector<std::string> CalculateCorner::GetAllFilePaths() const {
	return this->ImagePathVector;
}

std::map<std::string, cv::Mat> CalculateCorner::GetProcessedImageByPath() const {
	return this->FindProcessedImageByPath;
}
