#pragma once
#include <memory>
#include "MissileStateMulti.h"
#include "MissileStateCharge.h"
#include "MissileStateUltimate.h"

enum class MissileType
{
	Multi,
	Charge,
	Ultimate
};

class MissileLauncher
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	std::shared_ptr<IMissileState> mi_state_;

	std::shared_ptr<MissileStateMulti> multi_missile_;
	std::shared_ptr<MissileStateCharge> charge_missile_;
	std::shared_ptr<MissileStateUltimate> ultimate_missile_;

public:

	MissileLauncher();

public:

	bool FireMissile(MissileType type, const XMFLOAT3 &launch_pos, MissileManager *ptr);
};
