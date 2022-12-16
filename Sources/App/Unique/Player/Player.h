#pragma once
#include <memory>
#include "../Reticle/Reticle.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "../Ultimate/UltimateManager.h"
#include "../Missile/MissileLauncher.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	static constexpr uint32_t invincible_time_ = 80;

private:

	// 描画データ
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	bool is_invincible_;			// 無敵状態かどうか
	bool taking_damage_trigger_;	// ダメージを受けた瞬間か
	bool is_triggering_ult_;		// ウルトを放った瞬間か
	uint32_t hp_;					// HP

	// 他クラス情報
	LockOnSystem *p_lockon_sys_;
	UltimateManager *p_ult_;

	// ミサイル発射関連
	std::unique_ptr<MissileLauncher> mi_launcher_;
	int32_t charge_time_;			// 発射に必要なチャージ時間
	int32_t count_;					// チャージカウント用

	// イージング用変数
	int32_t ease_rot_right_;		// 右傾倒時の姿勢遷移用
	int32_t ease_rot_left_;			// 左傾倒時の姿勢遷移用
	int32_t ease_reset_rot_;		// 姿勢リセット用

	bool is_already_;
	bool is_released;

	float rot_angle_;				// 傾倒する角度

public:

	Player();
	~Player();

public:

	static void LoadResources();

	inline const bool &GetIsInvincible() { return is_invincible_; }
	inline const bool &GetIsTriggeringUlt() { return is_triggering_ult_; }
	inline const uint32_t &GetHp() { return hp_; }
	inline const int32_t &GetChargeCount() { return count_; }
	inline const int32_t &GetMaxChargeTime() { return charge_time_; }

	void Initialize() override;
	void Initialize(LockOnSystem *lockon_sys, UltimateManager *ult, XMFLOAT3 pos);
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	void ChargeMissile();
	void TakeDamage();
	void CountInvincibleTime();
	void Move(float speed);
	void MoveXY(float speed);
	void MoveXZ(float speed);
	void RotationY(float speed);
	void MoveForwardAuto();

	void RotPoseLeft(XMFLOAT3 &rot);
	void RotPoseRight(XMFLOAT3 &rot);
	void ResetRotPose(XMFLOAT3 &rot);
};
