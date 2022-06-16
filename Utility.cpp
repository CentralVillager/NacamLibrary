#include "Utility.h"
#include <random>

using namespace DirectX;

XMFLOAT3 NacamUtility::GenerateRandom(XMFLOAT3 min, XMFLOAT3 max) {

	// シード値
	std::random_device seed;

	// メルセンヌ・ツイスター疑似乱数列生成器
	std::mt19937 engine(seed());

	// 指定範囲内で離散一様分布
	std::uniform_int_distribution<> random_x(min.x, max.x);
	std::uniform_int_distribution<> random_y(min.y, max.y);
	std::uniform_int_distribution<> random_z(min.z, max.z);

	// 生成結果をXMFLOAT3で返す
	return { static_cast<float>(random_x(engine)), static_cast<float>(random_y(engine)),  static_cast<float>(random_z(engine)) };
}

int NacamUtility::GenerateRandom(int min, int max) {

	// シード値
	std::random_device seed;

	// メルセンヌ・ツイスター疑似乱数列生成器
	std::mt19937 engine(seed());

	// 指定範囲内で離散一様分布
	std::uniform_int_distribution<> random(min, max);

	// 生成結果をintで返す
	return random(engine);
}

float NacamUtility::GenerateRandom(float min, float max) {

	// シード値
	std::random_device seed;

	// メルセンヌ・ツイスター疑似乱数列生成器
	std::mt19937 engine(seed());

	// 指定範囲内で離散一様分布
	std::uniform_int_distribution<> random(min, max);

	// 生成結果をfloatで返す
	return random(engine);
}
