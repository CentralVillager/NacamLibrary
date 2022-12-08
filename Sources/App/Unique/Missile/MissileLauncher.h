#pragma once
#include <memory>
#include "IMissileLaunchState.h"

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

	std::shared_ptr<IMissileLaunchState> state_;

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
