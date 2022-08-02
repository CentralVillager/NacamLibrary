#pragma once
#include <DirectXMath.h>

namespace NacamUtility {

	using XMFLOAT3 = DirectX::XMFLOAT3;

	/// <summary>
	/// 指定された範囲内で乱数を返します。
	///	</summary>
	XMFLOAT3 GenerateRandom(XMFLOAT3 min, XMFLOAT3 max);

	/// <summary>
	/// 指定された範囲内で乱数を返します。
	/// </summary>
	int GenerateRandom(int min, int max);

	/// <summary>
	/// 指定された範囲内で乱数を返します。
	/// </summary>
	float GenerateRandom(float min, float max);

	/// <summary>
	/// 0~256基準の値を0~1基準に変換します。
	/// </summary>
	/// <param name="value_of_256"></param>
	/// <returns></returns>
	inline float Convert256to01(int value_of_256) {

		return value_of_256 / 256.0f;
	}
}
