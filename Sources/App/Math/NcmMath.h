#pragma once
#include <DirectXMath.h>

namespace NcmMath
{
	/// <summary>
	/// �~����
	/// </summary>
	static constexpr float PI = 3.14159265358979f;

	/// <summary>
	/// �ʓx��x���֕ϊ����܂��B
	/// </summary>
	/// <param name="radian"></param>
	static void ToDegree(float *radian)
	{
		*radian *= (180.0f / PI);
	}

	/// <summary>
	/// �����x�N�g�������]�p�����߂܂��B
	/// </summary>
	/// <param name="src">�����������I�u�W�F�N�g�̃x�N�g��</param>
	/// <param name="dist">�������������̃x�N�g��</param>
	/// <returns></returns>
	static float LookAt(const DirectX::XMVECTOR &src, const DirectX::XMVECTOR &dist)
	{
		// �����x�N�g�������߂�
		DirectX::XMVECTOR vec =
		{
			(src.m128_f32[0] - dist.m128_f32[0]),
			(src.m128_f32[1] - dist.m128_f32[1]),
			(src.m128_f32[2] - dist.m128_f32[2])
		};

		// �p�x�����߂�
		float angle = (float)(atan2(vec.m128_f32[0], vec.m128_f32[2]));

		// �x���ɕϊ�
		ToDegree(&angle);

		return angle + PI;
	}

	/// <summary>
	/// �����x�N�g�������]�p�����߂܂��B
	/// </summary>
	/// <param name="vec">�v�Z�ς݂̕����x�N�g��</param>
	/// <returns></returns>
	static float LookAt(const DirectX::XMVECTOR &vec)
	{
		// �p�x�����߂�
		float angle = (float)(atan2(vec.m128_f32[0], vec.m128_f32[2]));

		// �x���ɕϊ�
		ToDegree(&angle);

		return angle + PI;
	}

	/// <summary>
	/// �����x�N�g�������]�p�����߂܂��B
	/// </summary>
	/// <param name="vec">�����x�N�g��</param>
	/// <returns></returns>
	static DirectX::XMFLOAT3 LookAt(const DirectX::XMFLOAT3 &vec)
	{
		using namespace DirectX;

		XMVECTOR l_vec = XMLoadFloat3(&vec);

		// �p�x�����߂�
		XMFLOAT3 angle{};
		angle.x = (float)(atan2(l_vec.m128_f32[1], l_vec.m128_f32[2]));
		angle.y = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[2]));
		angle.z = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[1]));

		// �x���ɕϊ�
		XMFLOAT3 degrees{};
		degrees.x = XMConvertToDegrees(angle.x);
		degrees.y = XMConvertToDegrees(angle.y);
		degrees.z = XMConvertToDegrees(angle.z);

		return XMFLOAT3(degrees.x + PI, degrees.y + PI, degrees.z + PI);
	}

	/// <summary>
	/// sin�g�����߂܂��B
	/// </summary>
	/// <param name="count"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	inline float GetSinWave(const int count, float time)
	{
		return (float)(sin(PI * 2.0f / (float)(count)*time));
	}

	/// <summary>
	/// 2�̃x�N�g���̂Ȃ��p�����߂܂��B
	/// </summary>
	/// <param name="vec1"></param>
	/// <param name="vec2"></param>
	/// <returns></returns>
	inline float GetAngleFromDoubleVec(DirectX::XMVECTOR vec1, DirectX::XMVECTOR vec2)
	{
		using namespace DirectX;

		// cos�������߂邽�߂̍ޗ�
		float dot = XMVector3Dot(vec1, vec2).m128_f32[0];
		float mi_len = XMVector3Length(vec1).m128_f32[0];
		float dest_len = XMVector3Length(vec2).m128_f32[0];

		// ���ςƃx�N�g���̒�����p����cos�������߂�
		float cos_theta = dot / (mi_len * dest_len);

		// �������߂�
		float theta = (float)(acos(cos_theta));

		// �x���ɕϊ�
		ToDegree(&theta);

		return theta;
	}
}
