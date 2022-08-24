#include "../Utility/Utility.h"
#include <random>

using namespace DirectX;

XMFLOAT3 NcmUtill::GenerateRandom(XMFLOAT3 min, XMFLOAT3 max) {

	// シード値
	std::random_device seed;

	// メルセンヌ・ツイスター疑似乱数列生成器
	std::mt19937 engine(seed());

	// 指定範囲内で離散一様分布
	std::uniform_real_distribution<float> random_x(min.x, max.x);
	std::uniform_real_distribution<float> random_y(min.y, max.y);
	std::uniform_real_distribution<float> random_z(min.z, max.z);

	// 生成結果をXMFLOAT3で返す
	return { float(random_x(engine)), float(random_y(engine)),  float(random_z(engine)) };
}

int NcmUtill::GenerateRandom(int min, int max) {

	// シード値
	std::random_device seed;

	// メルセンヌ・ツイスター疑似乱数列生成器
	std::mt19937 engine(seed());

	// 指定範囲内で離散一様分布
	std::uniform_int_distribution<int> random(min, max);

	// 生成結果をintで返す
	return random(engine);
}

float NcmUtill::GenerateRandom(float min, float max) {

	// シード値
	std::random_device seed;

	// メルセンヌ・ツイスター疑似乱数列生成器
	std::mt19937 engine(seed());

	// 指定範囲内で離散一様分布
	std::uniform_real_distribution<float> random(min, max);

	// 生成結果をfloatで返す
	return float(random(engine));
}
