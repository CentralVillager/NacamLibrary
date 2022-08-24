#include "../Utility/Utility.h"
#include <random>

using namespace DirectX;

XMFLOAT3 NcmUtill::GenerateRandom(XMFLOAT3 min, XMFLOAT3 max) {

	// �V�[�h�l
	std::random_device seed;

	// �����Z���k�E�c�C�X�^�[�^�������񐶐���
	std::mt19937 engine(seed());

	// �w��͈͓��ŗ��U��l���z
	std::uniform_real_distribution<float> random_x(min.x, max.x);
	std::uniform_real_distribution<float> random_y(min.y, max.y);
	std::uniform_real_distribution<float> random_z(min.z, max.z);

	// �������ʂ�XMFLOAT3�ŕԂ�
	return { float(random_x(engine)), float(random_y(engine)),  float(random_z(engine)) };
}

int NcmUtill::GenerateRandom(int min, int max) {

	// �V�[�h�l
	std::random_device seed;

	// �����Z���k�E�c�C�X�^�[�^�������񐶐���
	std::mt19937 engine(seed());

	// �w��͈͓��ŗ��U��l���z
	std::uniform_int_distribution<int> random(min, max);

	// �������ʂ�int�ŕԂ�
	return random(engine);
}

float NcmUtill::GenerateRandom(float min, float max) {

	// �V�[�h�l
	std::random_device seed;

	// �����Z���k�E�c�C�X�^�[�^�������񐶐���
	std::mt19937 engine(seed());

	// �w��͈͓��ŗ��U��l���z
	std::uniform_real_distribution<float> random(min, max);

	// �������ʂ�float�ŕԂ�
	return float(random(engine));
}
