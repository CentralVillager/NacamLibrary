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

	/// <summary>
	/// 方向ベクトルから回転角を求めます。
	/// </summary>
	/// <param name="vec">方向ベクトル</param>
	/// <returns></returns>
	static DirectX::XMFLOAT3 LookAt(const DirectX::XMFLOAT3 &vec)
	{
		using namespace DirectX;

		XMVECTOR l_vec = XMLoadFloat3(&vec);

		// 角度を求める
		XMFLOAT3 angle{};
		angle.x = (float)(atan2(l_vec.m128_f32[1], l_vec.m128_f32[2]));
		angle.y = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[2]));
		angle.z = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[1]));

		// 度数に変換
		XMFLOAT3 degrees{};
		degrees.x = XMConvertToDegrees(angle.x);
		degrees.y = XMConvertToDegrees(angle.y);
		degrees.z = XMConvertToDegrees(angle.z);

		return XMFLOAT3(degrees.x + PI, degrees.y + PI, degrees.z + PI);
	}

	/// <summary>
	/// sin波を求めます。
	/// </summary>
	/// <param name="count"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	inline float GetSinWave(const int count, float time)
	{
		return (float)(sin(PI * 2.0f / (float)(count)*time));
	}

	/// <summary>
	/// 2つのベクトルのなす角を求めます。
	/// </summary>
	/// <param name="vec1"></param>
	/// <param name="vec2"></param>
	/// <returns></returns>
	inline float GetAngleFromDoubleVec(DirectX::XMVECTOR vec1, DirectX::XMVECTOR vec2)
	{
		using namespace DirectX;

		// cosΘを求めるための材料
		float dot = XMVector3Dot(vec1, vec2).m128_f32[0];
		float mi_len = XMVector3Length(vec1).m128_f32[0];
		float dest_len = XMVector3Length(vec2).m128_f32[0];

		// 内積とベクトルの長さを用いてcosΘを求める
		float cos_theta = dot / (mi_len * dest_len);

		// Θを求める
		float theta = (float)(acos(cos_theta));

		// 度数に変換
		ToDegree(&theta);

		return theta;
	}
}
