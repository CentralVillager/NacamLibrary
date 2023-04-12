#pragma once
#include <DirectXMath.h>
#include "IMissileLaunchState.h"
#include "../../Lib/Utility/NcmColor.h"

class MissileStateUltimate : public IMissileLaunchState
{
	// ミサイルの総発射数
	static constexpr uint32_t ULT_LAUNCH_NUM_ = 30;

public:

	bool FireMissile(LaunchedBy launcher, const DirectX::XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		/* プレイヤーのみが使用する想定なのでlauncherは使用しない */

		// 総ターゲット数
		static uint32_t all_tgt_num = 0;
		// ターゲット一体につくミサイルの数
		static uint32_t contract_per_tgt = 0;
		// 参照するターゲットの位置の記録
		static uint32_t tgt_location_ref = 0;
		// 発射間隔
		static const uint32_t DELAY = 2;
		// カウント
		static uint32_t count = 1;
		// 既発射数
		static uint32_t launched = 0;
		// ULT発動時の初回の発射か
		static bool is_first_launch = true;

		// ULT発動時の初回の発射なら
		if (is_first_launch)
		{
			/* 初回発射時のデータを元にする */
			// 総ターゲット数を取得
			all_tgt_num = ptr->GetLockOnSys()->GetMaxTgtNum();
			// ターゲット一体につくミサイルの数を算出(総発射予定数 / 総ターゲット数)
			contract_per_tgt = ULT_LAUNCH_NUM_ / all_tgt_num;
			// 初回フラグを降ろす
			is_first_launch = false;
			// 参照場所をリセット
			tgt_location_ref = 0;
		}

		// カウントを減らす
		count--;

		// カウントが1以上なら
		if (count >= 1)
		{
			// 以降の処理をスルー
			return false;
		}

		// ターゲットの参照を取得
		auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), tgt_location_ref);

		// ターゲットデータを取得
		MissileParam param{};

		if (launched % 2 == 0)
		{
			param.accuracy_type = HomingAccuracy::Linear;
			param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-0.2f, -0.2f, 0), DirectX::XMFLOAT3(0.2f, 0.2f, 0));
		}
		else
		{
			param.accuracy_type = HomingAccuracy::High;
			param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-1.5f, -1.5f, 0), DirectX::XMFLOAT3(1.5f, 1.5f, 0));
		}
		param.tgt_pos = itr->pos;
		param.tgt_id = itr->id;

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

		// パラメータを設定
		param.speed = 10.0f;
		param.pos = launch_pos;
		DirectX::XMStoreFloat3(&param.vel, direction_vec);
		// tgt_pos	 は下で設定
		// tgt_index は下で設定
		param.use_homing_time = false;
		param.detection_range = 1000.0f;
		param.init_straight_time = 0;
		param.life = NcmUtill::GenerateRandom(200, 300);
		param.trail_color = NcmColor::TRAIL_WHITE;

		// 最大ロックオン数を10に設定
		ptr->GetLockOnSys()->SetMaxTgtNum(10);

		// ミサイルを追加
		ptr->AddMissile(param);

		// 発射回数を加算
		launched++;

		// 参照場所を進ませる
		tgt_location_ref++;

		// 参照場所が総ターゲット数を越したら
		if (tgt_location_ref > all_tgt_num)
		{
			// 参照場所を最初からにする
			tgt_location_ref = 0;
		}

		// 最大ターゲット数をデフォルトに戻す
		ptr->GetLockOnSys()->SetMaxTgtNum(LockOnSystem::DEFAULT_TGT_NUM_);

		// 発射間隔を持たせる
		count = DELAY;

		// 総発射数が既定の数に達したら
		if (launched >= ULT_LAUNCH_NUM_)
		{
			// パラメータをリセット
			launched = 0;
			count = 1;

			// 終了を知らせる
			return true;
		}

		// 続行中を知らせる
		return false;
	}
};
