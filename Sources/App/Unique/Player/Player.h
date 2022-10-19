#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Sources/Lib/Model/Model.h"
#include "../Sources/Lib/Object3d/Object3d.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../Reticle/Reticle.h"
#include "../Missile/MissileManager.h"
#include "../LockOnSystem/LockOnSystem.h"

class Player
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// 描画データ
	static std::shared_ptr<Model> model_;
	static std::shared_ptr<Model> coll_model_;
	std::shared_ptr<Object3d> object_;
	std::shared_ptr<Object3d> sphere_obj_;

	// 当たり判定関連
	Sphere coll_;
	const float COLL_RADIUS_ = 2.0f;

	// 他クラス情報
	MissileManager *mi_mgr_;
	LockOnSystem *lockon_sys_;

	// ミサイル発射関連
	int charge_time = 40;
	int count = 0;

	// イージング用変数
	int ease_rot_right_;
	int ease_rot_left_;
	int ease_reset_rot_;

	bool is_already_ = false;
	bool is_released = false;

public:

	Player();
	~Player();

public:

	static void LoadResources();

	const XMFLOAT3 &GetPos() { return object_->GetPosition(); }

	void Initialize(MissileManager *mi_mgr, LockOnSystem *lockon_sys);
	void Finalize();
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	void FireMissile();
	void ChargeMissile();
	void Move(float speed);
	void MoveXY(float speed);
	void MoveXZ(float speed);
	void UpdateCollision();

	void RotPoseLeft();
	void RotPoseRight();
	void ResetRotPose();
};
