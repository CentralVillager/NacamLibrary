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
	/// ミサイルを発射する
	/// </summary>
	/// <param name="type">発射の仕方</param>
	/// <param name="launch_pos">発射場所</param>
	/// <param name="vec">初期ベクトル</param>
	/// <param name="ptr">MissileManagerのポインタ</param>
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
