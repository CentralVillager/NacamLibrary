#pragma once
#include <DirectXMath.h>
#include "../Missile/MissileManager.h"

class IMissileState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	virtual bool FireMissile(const XMFLOAT3 &launch_pos, MissileManager *ptr) = 0;
};
