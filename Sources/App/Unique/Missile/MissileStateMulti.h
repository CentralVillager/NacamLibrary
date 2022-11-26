#pragma once
#include "IMissileState.h"

class MissileStateMulti : public IMissileState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		MissileArgs l_args{};
		l_args.pos = launch_pos;
		l_args.vel = XMFLOAT3(0, 0, 1.0f);
		//l_args.acc = XMFLOAT3(0, -1.0f, 0);
		// acc はMissileManagerで設定
		// tgt_pos はMissileManagerで設定
		// tgt_index はMissileManagerで設定
		l_args.detection_range = 1000.0f;
		l_args.init_straight_time_ = 0;
		l_args.life = 200;

		ptr->FireMultiMissile(l_args, 4);

		return false;	// 共通化のためのやむなしリターン
	}
};
