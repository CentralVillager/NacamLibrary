#pragma once
#include <memory>
#include "IMissileLaunchState.h"
#include "MissileStateCharge.h"
#include "MissileStateUltimate.h"
#include "MissileStateMonoCircuse.h"
#include "MissileManager.h"

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

	inline static MissileManager *mi_ptr_;

	std::shared_ptr<IMissileLaunchState> state_;

public:

	static void SetMissileMgrPtr(MissileManager *ptr) { mi_ptr_ = ptr; }

public:

	/// <summary>
	/// ミサイルを発射する
	/// </summary>
	/// <param name="type">発射の仕方</param>
	/// <param name="launch_pos">発射場所</param>
	/// <param name="dest_pos">目標位置</param>
	/// <param name="ptr">MissileManagerのポインタ</param>
	/// <returns></returns>
	bool FireMissile(MissileType type, const XMFLOAT3 &launch_pos, const XMFLOAT3 &dest_pos)
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

		return state_->FireMissile(launch_pos, dest_pos, mi_ptr_);
	}
};
