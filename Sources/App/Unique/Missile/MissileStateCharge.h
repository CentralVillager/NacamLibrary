#pragma once
#include "IMissileLaunchState.h"

class MissileStateCharge : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, const XMFLOAT3 &vec, MissileManager *ptr) override
	{
		MissileParam param{};
		param.pos = launch_pos;
		param.vel = vec;
		// acc はMissileManagerで設定
		// tgt_pos はMissileManagerで設定
		// tgt_index はMissileManagerで設定
		param.detection_range = 1000.0f;
		param.init_straight_time = 0;
		param.life = 300;

		// ロック上限数まで
		for (UINT i = 0; i < ptr->GetLockOnSys()->GetCurrentTgtNum(); i++)
		{
			auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), i);

			// ターゲットデータを取得
			param.tgt_pos = itr->pos;
			param.tgt_id = itr->id;

			// ミサイルを追加
			ptr->AddMissile(param);
		}

		return false;	// 共通化のためのやむなしリターン
	}
};
