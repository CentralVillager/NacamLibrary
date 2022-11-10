#pragma once
#include <DirectXMath.h>
#include <limits>

namespace NcmUtill
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

#pragma region Random

	/// <summary>
	/// �w�肳�ꂽ�͈͓��ŗ�����Ԃ��܂��B
	///	</summary>
	XMFLOAT3 GenerateRandom(XMFLOAT3 min, XMFLOAT3 max);

	/// <summary>
	/// �w�肳�ꂽ�͈͓��ŗ�����Ԃ��܂��B
	/// </summary>
	int GenerateRandom(int min, int max);

	/// <summary>
	/// �w�肳�ꂽ�͈͓��ŗ�����Ԃ��܂��B
	/// </summary>
	float GenerateRandom(float min, float max);

#pragma endregion

#pragma region Convert

	/// <summary>
	/// 0~256��̒l��0~1��ɕϊ����܂��B
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
	/// ���̒l���v���X���ǂ����𔻒肵�܂��B
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline static bool IsPlus(const float &value)
	{
		if (value >= 0) { return true; }

		return false;
	}

	/// <summary>
	/// ���̒l���}�C�i�X���ǂ����𔻒肵�܂��B
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
	/// �C�e���[�^��n��i�܂��܂��B
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
	/// �~����
	/// </summary>
	static constexpr float NcmPI = 3.14159265358979f;

	/// <summary>
	/// �ʓx��x���֕ϊ����܂��B
	/// </summary>
	/// <param name="radian"></param>
	static void ToDegree(float *radian)
	{
		*radian *= (180.0f / NcmPI);
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

		return angle + NcmPI;
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

		return angle + NcmPI;
	}

	static DirectX::XMFLOAT3 LookAt(const DirectX::XMFLOAT3 &vec)
	{
		using namespace DirectX;

		XMVECTOR l_vec = XMLoadFloat3(&vec);

		// �p�x�����߂�
		XMFLOAT3 angle;
		angle.x = (float)(atan2(l_vec.m128_f32[1], l_vec.m128_f32[2]));
		angle.y = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[2]));
		angle.z = (float)(atan2(l_vec.m128_f32[0], l_vec.m128_f32[1]));

		// �x���ɕϊ�
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
