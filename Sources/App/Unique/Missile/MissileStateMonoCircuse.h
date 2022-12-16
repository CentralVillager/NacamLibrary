#pragma once
#include "IMissileLaunchState.h"

class MissileStateMonoCircuse : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, const XMFLOAT3 &vec, MissileManager *ptr) override
	{
		// パラメータを設定
		MissileParam param{};
		param.pos = launch_pos;
		param.vel = vec;
		// 加速度をランダムに設定
		param.acc = NcmUtill::GenerateRandom(XMFLOAT3(-1.5f, -1.5f, 0), XMFLOAT3(1.5f, 1.5f, 0));
		// tgt_pos	 は下で設定
		// tgt_index は下で設定
		param.detection_range = 1000.0f;
		param.init_straight_time = 0;
		param.life = 300;

		// ターゲットの参照を取得
		auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), 0);

		// ターゲットデータを取得
		param.tgt_pos = itr->pos;
		param.tgt_id = itr->id;

		// ミサイルを追加
		ptr->AddMissile(param);

		return false;
	}
};
