#pragma once
#include <memory>
#include "../Reticle/Reticle.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "../Ultimate/UltimateManager.h"
#include "../Missile/MissileLauncher.h"

class Camera;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

private:

	// 無敵時間
	static constexpr uint32_t INVINCIBLE_TIME_ = 80;

	// 移動可能範囲
	static constexpr XMFLOAT2 MOVABLE_RANGE_{ 20.0f, 15.0f };

	static constexpr float NORMAL_FOV_ = 60.0f;	// 通常視野角
	static constexpr float ACCEL_FOV_ = 70.0f;	// 加速時視野角

	static Camera *cam_ptr_;

private:

	// 描画データ
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	float normal_speed_;			// 通常時速度

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
	ncm_ehandle ease_rot_right_;	// 右傾倒時の姿勢遷移用
	ncm_ehandle ease_rot_left_;		// 左傾倒時の姿勢遷移用
	ncm_ehandle ease_reset_rot_;	// 姿勢リセット用

	ncm_ehandle ease_move_left_;	// 右移動時の位置遷移用
	ncm_ehandle ease_move_right_;	// 左移動時の位置遷移用
	ncm_ehandle ease_move_reset_;	// リセット用

	ncm_ehandle acc_speed_;			// 加速時速度
	ncm_ehandle dec_speed_;			// 減速時速度

	ncm_ehandle fov_acc_value_;		// 加速時視野角遷移
	ncm_ehandle fov_dec_value_;		// 減速時視野角遷移

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
	inline void SetCamPtr(Camera *p) { cam_ptr_ = p; }

	void Initialize() override;
	void Initialize(LockOnSystem *lockon_sys, UltimateManager *ult, const XMFLOAT3 &pos);
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// ミサイルをチャージする
	/// </summary>
	void ChargeMissile();

	/// <summary>
	/// ダメージを受ける
	/// </summary>
	void TakeDamage();

	/// <summary>
	/// 無敵時間をカウントする
	/// </summary>
	void CountInvincibleTime();

	/// <summary>
	/// 横・奥方向へのコントロール
	/// </summary>
	/// <param name="speed">速度</param>
	void MoveXZ(float speed);

	/// <summary>
	/// 横・縦方向へのコントロール
	/// </summary>
	/// <param name="speed">速度</param>
	void MoveXY(float speed);

	/// <summary>
	/// 回頭処理
	/// </summary>
	/// <param name="speed">速度</param>
	void RotationY(float speed);

	/// <summary>
	/// 自動で前に進ませる
	/// </summary>
	/// <param name="speed">速度</param>
	/// <returns></returns>
	volatile void MoveForwardAuto(float speed);

	/// <summary>
	/// 前に進ませる
	/// </summary>
	/// <param name="speed">速度</param>
	void MoveZ(float speed);

	/// <summary>
	/// 加速させる
	/// </summary>
	void Accelerate();

	void RotPoseLeft(XMFLOAT3 &rot);
	void RotPoseRight(XMFLOAT3 &rot);
	void ResetRotPose(XMFLOAT3 &rot);
};
