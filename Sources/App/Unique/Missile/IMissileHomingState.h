#pragma once
#include <DirectXMath.h>

/// <summary>
/// �~�T�C���ǔ��p�^�[���̊��N���X
/// </summary>
class IMissileHomingState
{
protected:

	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	virtual void HomingTarget(XMFLOAT3 dest_pos) = 0;
};
