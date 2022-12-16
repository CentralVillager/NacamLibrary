#pragma once
#include "IMissileLaunchState.h"

class MissileStateUltimate : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// ミサイルの総発射数
	static constexpr uint32_t ULT_LAUNCH_NUM_ = 20;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, const XMFLOAT3 &vec, MissileManager *ptr) override
	{
		// 総ターゲット数
		static uint32_t all_tgt_num_ = 0;
		// ターゲット一体につくミサイルの数
		static uint32_t contract_per_tgt_ = 0;
		// 参照するターゲットの位置の記録
		static uint32_t tgt_location_ref_ = 0;
		// 発射間隔
		static const uint32_t DELAY = 3;
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
			all_tgt_num_ = ptr->GetLockOnSys()->GetMaxTgtNum();
			// ターゲット一体につくミサイルの数を算出(総発射予定数 / 総ターゲット数)
			contract_per_tgt_ = ULT_LAUNCH_NUM_ / all_tgt_num_;
			// 初回フラグを降ろす
			is_first_launch = false;
			// 参照場所をリセット
			tgt_location_ref_ = 0;
		}

		// カウントを減らす
		count--;

		// カウントが1以上なら
		if (count >= 1)
		{
			// 以降の処理をスルー
			return false;
		}

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

		// 最大ロックオン数を10に設定
		ptr->GetLockOnSys()->SetMaxTgtNum(10);

		// ターゲットの参照を取得
		auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), tgt_location_ref_);

		// ターゲットデータを取得
		param.tgt_pos = itr->pos;
		param.tgt_id = itr->id;

		// ミサイルを追加
		ptr->AddMissile(param);

		// 発射回数を加算
		launched++;

		// 参照場所を進ませる
		tgt_location_ref_++;

		// 参照場所が総ターゲット数を越したら
		if (tgt_location_ref_ > all_tgt_num_)
		{
			// 参照場所を最初からにする
			tgt_location_ref_ = 0;
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
