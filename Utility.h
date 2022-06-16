#pragma once
#include <DirectXMath.h>

namespace NacamUtility {

	using XMFLOAT3 = DirectX::XMFLOAT3;

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
}
