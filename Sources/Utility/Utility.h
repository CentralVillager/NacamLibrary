#pragma once
#include <DirectXMath.h>

namespace NcmUtill
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

#pragma region Random

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

#pragma endregion

#pragma region Convert

	/// <summary>
	/// 0~256基準の値を0~1基準に変換します。
	/// </summary>
	/// <param name="value_of_256"></param>
	/// <returns></returns>
	static inline float Convert256to01(int value_of_256)
	{
		return value_of_256 / 256.0f;
	}

	/// <summary>
	/// XMVECTORの値をXMFLOAT3に変換します。
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	static inline const XMFLOAT3 &ToFloat3(const XMVECTOR &v)
	{
		return XMFLOAT3(v.m128_f32[0], v.m128_f32[1], v.m128_f32[2]);
	}

#pragma endregion

#pragma region Number

	/// <summary>
	/// その値がプラスかどうかを判定します。
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	static inline bool IsPlus(const float &value)
	{
		if (value >= 0) { return true; }

		return false;
	}

	/// <summary>
	/// その値がマイナスかどうかを判定します。
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	static inline bool IsMinus(const float &value)
	{
		if (value < 0) { return true; }

		return false;
	}

	static inline bool IsZero(const int &num)
	{
		if (num == 0) { return true; }

		return false;
	}

#pragma endregion
}
