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

	std::shared_ptr<IMissileState> state_;

	std::shared_ptr<MissileStateMulti> multi_missile_;
	std::shared_ptr<MissileStateCharge> charge_missile_;
	std::shared_ptr<MissileStateUltimate> ultimate_missile_;

public:

	/// <summary>
	/// �~�T�C���𔭎˂���
	/// </summary>
	/// <param name="type">���˕���</param>
	/// <param name="launch_pos">���˒n�_</param>
	/// <param name="ptr">MissileManager�̃|�C���^</param>
	/// <returns></returns>
	bool FireMissile(MissileType type, const XMFLOAT3 &launch_pos, MissileManager *ptr);
};
