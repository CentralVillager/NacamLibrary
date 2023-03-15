#pragma once
#include "IMissileLaunchState.h"
#include "../../Utility/NcmColor.h"

class MissileStateCharge : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(LaunchedBy launcher, const DirectX::XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		MissileParam param{};

		switch (launcher)
		{
		case LaunchedBy::Enemy:

			// IDをプレイヤーのものとする
			param.tgt_id = (int32_t)(TargetIs::Player);
			// tgt_pos は後に処理する
			break;

		case LaunchedBy::Player:
		{
			// ターゲットの参照を取得
			auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), 0);

			if (ptr->GetLockOnSys()->GetTgtList().empty())
			{
				return false;
			}

			// 取得したIDが敵最大数より大きかったら
			if (ptr->GetLockOnSys()->GetTgtList().size() < itr->id)
			{
				// その敵は存在しない
				param.tgt_id = (int32_t)(NacamError::NonDetected);
				param.tgt_pos = DirectX::XMFLOAT3();
			}
			else
			{
				// ターゲットデータを取得
				param.tgt_pos = itr->pos;
				param.tgt_id = itr->id;
			}
		}
		break;

		default:
			break;
		}

		// XMVECTORへ変換
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

		param.speed = 4.0f;
		param.pos = launch_pos;
		DirectX::XMStoreFloat3(&param.vel, direction_vec);
		// tgt_pos は下で設定
		// tgt_id は下で設定
		param.detection_range = 1000.0f;
		param.use_homing_time = false;
		param.init_straight_time = 0;
		param.life = 300;
		param.trail_color = NcmColor::TRAIL_WHITE;

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
