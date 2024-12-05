
enum __declspec(dllexport)RadialDistortionType{
	// �������
	TWO_COEFFICIENTS, // ʹ�� 2 ���������ϵ����K1, K2��
	THREE_COEFFICIENTS // ʹ�� 3 ���������ϵ����K1, K2, K3��
};

struct __declspec(dllexport)CalibrationOptions {
	RadialDistortionType RadialDistortion = THREE_COEFFICIENTS;
	bool UseTangentialDistortion = false; // �Ƿ�ʹ���������
	bool OptimizeParameters = false; // �Ƿ��Ż���������ֵ��֪ʱ���ã�
	bool SkewEnabled = false; // �Ƿ�����б��У��
};
