#pragma once
#include <DirectXMath.h>

namespace NcmMath
{
	/// <summary>
	/// 円周率
	/// </summary>
	static constexpr float PI = 3.14159265358979f;

	/// <summary>
	/// 弧度を度数へ変換します。
	/// </summary>
	/// <param name="radian"></param>
	static void ToDegree(float *radian)
	{
		*radian *= (180.0f / PI);
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

		return angle + PI;
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

		return angle + PI;
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

		return XMFLOAT3(degrees.x + PI, degrees.y + PI, degrees.z + PI);
	}

	inline float GetSinWave(const int count, float time)
	{
		return (float)(sin(PI * 2.0f / (float)(count)*time));
	}
}
