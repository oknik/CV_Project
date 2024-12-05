
enum __declspec(dllexport)RadialDistortionType{
	// 径向畸变
	TWO_COEFFICIENTS, // 使用 2 个径向畸变系数（K1, K2）
	THREE_COEFFICIENTS // 使用 3 个径向畸变系数（K1, K2, K3）
};

struct __declspec(dllexport)CalibrationOptions {
	RadialDistortionType RadialDistortion = THREE_COEFFICIENTS;
	bool UseTangentialDistortion = false; // 是否使用切向畸变
	bool OptimizeParameters = false; // 是否优化参数（初值已知时启用）
	bool SkewEnabled = false; // 是否允许斜度校正
};
