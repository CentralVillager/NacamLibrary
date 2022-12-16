#pragma once
#include <memory>
#include "IMissileLaunchState.h"
#include "MissileStateCharge.h"
#include "MissileStateUltimate.h"
#include "MissileStateMonoCircuse.h"

enum class MissileType
{
	Charge,
	Ultimate,
	Mono
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
	/// <param name="type">���˂̎d��</param>
	/// <param name="launch_pos">���ˏꏊ</param>
	/// <param name="vec">�����x�N�g��</param>
	/// <param name="ptr">MissileManager�̃|�C���^</param>
	/// <returns></returns>
	bool FireMissile(MissileType type, const XMFLOAT3 &launch_pos, const XMFLOAT3 &vec, MissileManager *ptr)
	{
		using enum MissileType;

		switch (type)
		{
		case Charge:

			state_ = make_shared<MissileStateCharge>();
			break;

		case Ultimate:

			state_ = make_shared<MissileStateUltimate>();
			break;

		case Mono:

			state_ = make_shared<MissileStateMonoCircuse>();
			break;

		default:
			break;
		}

		return state_->FireMissile(launch_pos, vec, ptr);
	}
};
