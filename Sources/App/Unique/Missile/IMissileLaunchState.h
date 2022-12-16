#pragma once
#include <DirectXMath.h>
#include "../Missile/MissileManager.h"
#include "../../../App/Utility/NcmUtil.h"
#include "../../../App/Unique/Player/Player.h"

/// <summary>
/// �~�T�C�����˃p�^�[���̊��N���X
/// </summary>
class IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	virtual bool FireMissile(const XMFLOAT3 &launch_pos, const XMFLOAT3 &dest_pos, MissileManager *ptr) = 0;
};
