#pragma once
#include "IMissileLaunchState.h"
#include "../../Utility/NcmColor.h"

class MissileStateForEnemy : public IMissileLaunchState
{
	bool FireMissile(LaunchedBy launcher, const DirectX::XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		MissileParam param{};
		param.type = MissileType::ForEnemy;

		switch (launcher)
		{
		case LaunchedBy::Enemy:

			// IDをプレイヤーのものとする
			param.tgt_id = (int32_t)(TargetIs::Player);
			param.tgt_pos = DirectX::XMFLOAT3();
			param.life = 200;
			break;

		default:
			break;
		}

		// パラメータを設定
		param.speed = NcmUtill::GenerateRandom(0.5f, 3.0f);
		param.pos = launch_pos;
		// 加速度をランダムに設定
		param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-1.5f, -1.5f, 0), DirectX::XMFLOAT3(1.5f, 1.5f, 0));
		param.detection_range = 1000.0f;
		param.use_homing_time = true;
		param.homing_time = NcmUtill::GenerateRandom(50, 100);
		param.init_straight_time = 0;
		param.trail_color = NcmColor::TRAIL_RED;

		// 射出位置と目標位置をXMVECTORへ変換
		DirectX::XMVECTOR launch = DirectX::XMLoadFloat3(&launch_pos);
		DirectX::XMVECTOR dest = DirectX::XMLoadFloat3(&param.tgt_pos);

		// 方向ベクトルを算出
		DirectX::XMVECTOR direction_vec =
		{
			(dest.m128_f32[0] - launch.m128_f32[0]),
			(dest.m128_f32[1] - launch.m128_f32[1]),
			(dest.m128_f32[2] - launch.m128_f32[2])
		};
		// 正規化
		direction_vec = DirectX::XMVector3Normalize(direction_vec);

		// 初期速度を設定
		DirectX::XMStoreFloat3(&param.vel, direction_vec);

		// ミサイルを追加
		ptr->AddMissile(param);

		return false;
	}
};
