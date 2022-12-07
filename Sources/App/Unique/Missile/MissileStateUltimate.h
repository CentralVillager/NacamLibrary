#pragma once
#include "IMissileLaunchState.h"

class MissileStateUltimate : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		static const uint32_t DELAY = 3;
		static uint32_t count = 1;
		static uint32_t launched = 0;
		count--;

		if (NcmUtill::IsZeroOrLess(count))
		{
			MissileArgs l_args{};
			l_args.pos = launch_pos;
			l_args.vel = XMFLOAT3(0, 0, 1.0f);
			//l_args.acc = XMFLOAT3(0, -1.0f, 0);
			// acc ‚ÍMissileManager‚Åİ’è
			// tgt_pos ‚ÍMissileManager‚Åİ’è
			// tgt_index ‚ÍMissileManager‚Åİ’è
			l_args.detection_range = 1000.0f;
			l_args.init_straight_time = 0;
			l_args.life = 200;

			launched++;

			ptr->FireUltimateMissile(l_args, launched);
			count = DELAY;
		}

		if (launched >= 20)
		{
			launched = 0;
			count = 1;

			return true;
		}

		return false;
	}
};
