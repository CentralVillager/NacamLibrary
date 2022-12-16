#pragma once
#include "IMissileLaunchState.h"
#include <DirectXMath.h>

class MissileStateMonoCircuse : public IMissileLaunchState
{
	bool FireMissile(const DirectX::XMFLOAT3 &launch_pos, const DirectX::XMFLOAT3 &dest_pos, MissileManager *ptr) override
	{
		// XMVECTORへ変換
		DirectX::XMVECTOR launch = DirectX::XMLoadFloat3(&launch_pos);
		DirectX::XMVECTOR dest = DirectX::XMLoadFloat3(&dest_pos);

		// 方向ベクトルを算出
		DirectX::XMVECTOR direction_vec =
		{
			(dest.m128_f32[0] - launch.m128_f32[0]),
			(dest.m128_f32[1] - launch.m128_f32[1]),
			(dest.m128_f32[2] - launch.m128_f32[2])
		};
		// 正規化
		direction_vec = DirectX::XMVector3Normalize(direction_vec);

		// パラメータを設定
		MissileParam param{};
		param.pos = launch_pos;
		DirectX::XMStoreFloat3(&param.vel, direction_vec);
		// 加速度をランダムに設定
		param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-1.5f, -1.5f, 0), DirectX::XMFLOAT3(1.5f, 1.5f, 0));
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
