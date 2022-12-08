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
	/// �~�T�C���𔭎˂���
	/// </summary>
	/// <param name="type">���˕���</param>
	/// <param name="launch_pos">���˒n�_</param>
	/// <param name="ptr">MissileManager�̃|�C���^</param>
	/// <returns></returns>
	bool FireMissile(MissileType type, const XMFLOAT3 &launch_pos, MissileManager *ptr);
};
