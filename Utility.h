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
}
