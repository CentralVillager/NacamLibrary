#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Sources/Lib/Object3d/Object3d.h"
#include "../Sources/Lib/Model/Model.h"
#include "../Sources/App/Particle/Emitter.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"

struct MissileArgs
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT3 acc;

	XMFLOAT3 tgt_pos;
	int tgt_id;

	float detection_range;
	UINT init_straight_time_;
	UINT life;
	bool is_validity = false;
	bool is_dead = true;
};

class Missile
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	// 描画用データ
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;
	std::unique_ptr<Object3d> object_;
	std::shared_ptr<Object3d> sphere_obj_;

	// 当たり判定
	Sphere coll_;
	const float COLL_RADIUS_ = 1.0f;

	// エミッター
	std::unique_ptr<Emitter> emitter_;
	MissileArgs mi_args_;
	float min_dist_;
	float rot_dead_zone_;

	UINT tgt_index_;

	LockOnSystem *lockon_sys_;

public:

	Missile();
	~Missile();

public:

	static void LoadResources();

	void Initialize(const MissileArgs &args, LockOnSystem *sys);
	void Finalize();
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	const Sphere &GetCollData() { return coll_; }
	const XMFLOAT3 &GetPos() { return object_->GetPosition(); }
	const bool &GetIsValidity() { return mi_args_.is_validity; }

	void SetMissileLife(const int &life) { mi_args_.life = life; }
	void SetTgtPos(const XMFLOAT3 pos) { mi_args_.tgt_pos = pos; }

	const bool IsDead() { return mi_args_.is_dead == true; }

	void Activate();
	void InvalidateMissile() { mi_args_.is_validity = false; }

	void MoveZ(float speed);
	void HomingTarget(const XMFLOAT3 &target_pos);
	void HomingTarget(EnemiesList &enemies);
	void TermEmitter();

private:

	void UpdateCollision();
	void UpdateEmitter();
};
