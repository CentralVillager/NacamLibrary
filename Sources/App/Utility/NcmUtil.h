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
	inline static bool IsPlus(const float &value)
	{
		if (value >= 0) { return true; }

		return false;
	}

	/// <summary>
	/// その値がマイナスかどうかを判定します。
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline static bool IsMinus(const float &value)
	{
		if (value < 0) { return true; }

		return false;
	}

	inline static bool IsZero(const int &num)
	{
		if (num == 0) { return true; }

		return false;
	}

	inline static bool IsZeroOrLess(const int &num)
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

namespace NcmMath
{
	/// <summary>
	/// 円周率
	/// </summary>
	static constexpr float NcmPI = 3.14159265358979f;

	/// <summary>
	/// 弧度を度数へ変換します。
	/// </summary>
	/// <param name="radian"></param>
	static void ToDegree(float *radian)
	{
		*radian *= (180.0f / NcmPI);
	}

	/// <summary>
	/// 方向ベクトルから回転角を求めます。
	/// </summary>
	/// <param name="src">向かせたいオブジェクトのベクトル</param>
	/// <param name="dist">向きたい方向のベクトル</param>
	/// <returns></returns>
	static float LookAt(const DirectX::XMVECTOR &src, const DirectX::XMVECTOR &dist)
	{
		// 方向ベクトルを求める
		DirectX::XMVECTOR vec =
		{
			(src.m128_f32[0] - dist.m128_f32[0]),
			(src.m128_f32[1] - dist.m128_f32[1]),
			(src.m128_f32[2] - dist.m128_f32[2])
		};

		// 角度を求める
		float angle = (float)(atan2(vec.m128_f32[0], vec.m128_f32[2]));

		// 度数に変換
		ToDegree(&angle);

		return angle + NcmPI;
	}

	/// <summary>
	/// 方向ベクトルから回転角を求めます。
	/// </summary>
	/// <param name="vec">計算済みの方向ベクトル</param>
	/// <returns></returns>
	static float LookAt(const DirectX::XMVECTOR &vec)
	{
		// 角度を求める
		float angle = (float)(atan2(vec.m128_f32[0], vec.m128_f32[2]));

		// 度数に変換
		ToDegree(&angle);

		return angle + NcmPI;
	}

	static DirectX::XMFLOAT3 LookAt(const DirectX::XMFLOAT3 &vec)
	{
		using namespace DirectX;

		XMVECTOR l_vec = XMLoadFloat3(&vec);

		// 角度を求める
		XMFLOAT3 angle;
		angle.x = (float)(atan2(l_vec.m128_f32[1], l_vec.m128_f32[2]));
		angle.y = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[2]));
		angle.z = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[1]));

		// 度数に変換
		/*ToDegree(&angle.x);
		ToDegree(&angle.y);
		ToDegree(&angle.z);*/

		XMFLOAT3 degrees{};
		degrees.x = XMConvertToDegrees(angle.x);
		degrees.y = XMConvertToDegrees(angle.y);
		degrees.z = XMConvertToDegrees(angle.z);

		return XMFLOAT3(degrees.x + NcmPI, degrees.y + NcmPI, degrees.z + NcmPI);
	}

	inline float GetSinWave(const int count, float time)
	{
		return (float)(sin(NcmPI * 2.0f / (float)(count)*time));
	}
}
