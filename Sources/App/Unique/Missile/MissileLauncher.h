#pragma once
#include <memory>
#include "IMissileLaunchState.h"
#include "MissileStateCharge.h"
#include "MissileStateUltimate.h"
#include "MissileStateMonoCircuse.h"
#include "MissileStateForEnemy.h"
#include "MissileManager.h"
#include "MissileDescs.h"

class Player;
class LockOnSystem;

/// <summary>
/// �~�T�C���̔���
/// </summary>
class MissileLauncher
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	inline static MissileManager *mi_ptr_;
	inline static LockOnSystem *lock_ptr_;

	std::shared_ptr<IMissileLaunchState> state_;

public:

	static void SetPtr(MissileManager *missile, LockOnSystem *lock)
	{
		mi_ptr_ = missile;
		lock_ptr_ = lock;
	}

public:

	/// <summary>
	/// �~�T�C���𔭎˂���
	/// </summary>
	/// <param name="type">���˕��@</param>	
	/// <param name="launcher">���˂���I�u�W�F�N�g�̎��</param>
	/// <param name="launch_pos">���ˏꏊ</param>
	/// <param name="dest_pos">�ڕW�ʒu</param>
	/// <param name="ptr">MissileManager�̃|�C���^</param>
	/// <returns></returns>
	bool FireMissile(MissileType type, LaunchedBy launcher, const XMFLOAT3 &launch_pos)
	{
		using enum MissileType;
		using enum LaunchedBy;

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

		case ForEnemy:

			state_ = make_shared<MissileStateForEnemy>();
			break;

		default:
			break;
		}

		return state_->FireMissile(launcher, launch_pos, mi_ptr_);
	}
};
