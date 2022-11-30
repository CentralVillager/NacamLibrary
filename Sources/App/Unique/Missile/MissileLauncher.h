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
	/// ミサイルを発射する
	/// </summary>
	/// <param name="type">発射方式</param>
	/// <param name="launch_pos">発射地点</param>
	/// <param name="ptr">MissileManagerのポインタ</param>
	/// <returns></returns>
	bool FireMissile(MissileType type, const XMFLOAT3 &launch_pos, MissileManager *ptr);
};
