#include "Utility.h"
#include <random>

using namespace DirectX;

XMFLOAT3 NacamUtility::GenerateRandom(XMFLOAT3 min, XMFLOAT3 max) {

	// �V�[�h�l
	std::random_device seed;

	// �����Z���k�E�c�C�X�^�[�^�������񐶐���
	std::mt19937 engine(seed());

	// �w��͈͓��ŗ��U��l���z
	std::uniform_int_distribution<> random_x(min.x, max.x);
	std::uniform_int_distribution<> random_y(min.y, max.y);
	std::uniform_int_distribution<> random_z(min.z, max.z);

	// �������ʂ�XMFLOAT3�ŕԂ�
	return { static_cast<float>(random_x(engine)), static_cast<float>(random_y(engine)),  static_cast<float>(random_z(engine)) };
}

int NacamUtility::GenerateRandom(int min, int max) {

	// �V�[�h�l
	std::random_device seed;

	// �����Z���k�E�c�C�X�^�[�^�������񐶐���
	std::mt19937 engine(seed());

	// �w��͈͓��ŗ��U��l���z
	std::uniform_int_distribution<> random(min, max);

	// �������ʂ�int�ŕԂ�
	return random(engine);
}

float NacamUtility::GenerateRandom(float min, float max) {

	// �V�[�h�l
	std::random_device seed;

	// �����Z���k�E�c�C�X�^�[�^�������񐶐���
	std::mt19937 engine(seed());

	// �w��͈͓��ŗ��U��l���z
	std::uniform_int_distribution<> random(min, max);

	// �������ʂ�float�ŕԂ�
	return random(engine);
}
