#pragma once
#include <DirectXMath.h>
#include <limits>

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
	inline static float Convert256to01(int value_of_256)
	{
		return value_of_256 / 256.0f;
	}

#pragma endregion

#pragma region Number

	/// <summary>
	/// その値がプラスかどうかを判定します。
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline static bool IsPlus(const float value)
	{
		if (value >= 0) { return true; }

		return false;
	}

	/// <summary>
	/// その値がマイナスかどうかを判定します。
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline static bool IsMinus(const float value)
	{
		if (value < 0) { return true; }

		return false;
	}

	inline static bool IsZero(const int num)
	{
		if (num == 0) { return true; }

		return false;
	}

	inline static bool IsZeroOrLess(const int num)
	{
		if (num <= 0) { return true; }

		return false;
	}

#pragma endregion

#pragma region Undifined

	/// <summary>
	/// イテレータをn回進ませます。
	/// </summary>
	/// <param name="itr"></param>
	/// <param name="n"></param>
	/// <returns></returns>
	inline static auto MoveIterator(auto itr, int n)
	{
		for (int i = 0; i < n; i++)
		{
			itr++;
		}

		return itr;
	}

#pragma endregion
}
