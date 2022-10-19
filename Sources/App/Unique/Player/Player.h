#pragma once
#include "../Reticle/Reticle.h"
#include "../Missile/MissileManager.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"

class Player : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// 描画データ
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	// 他クラス情報
	MissileManager *mi_mgr_;
	LockOnSystem *lockon_sys_;

	// ミサイル発射関連
	int charge_time_;
	int count_;

	// イージング用変数
	int ease_rot_right_;
	int ease_rot_left_;
	int ease_reset_rot_;

	bool is_already_;
	bool is_released;

public:

	Player();
	~Player();

public:

	static void LoadResources();

	void Initialize() override;
	void Initialize(MissileManager *mi_mgr, LockOnSystem *lockon_sys);
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	void FireMissile();
	void ChargeMissile();
	void Move(float speed);
	void MoveXY(float speed);
	void MoveXZ(float speed);

	void RotPoseLeft();
	void RotPoseRight();
	void ResetRotPose();
};
