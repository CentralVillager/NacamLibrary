#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Model/Model.h"
#include "../Object3d/Object3d.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Unique/Reticle.h"
#include "../Unique/MissileManager.h"
#include "../Unique/LockOnSystem.h"

class Player
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	static std::shared_ptr<Model> model_;
	static std::shared_ptr<Model> sphere_model_;
	std::shared_ptr<Object3d> object_;
	std::shared_ptr<Object3d> sphere_obj_;

	Sphere coll_;
	const float COLL_RADIUS_ = 2.0f;
	MissileManager *mi_mgr_;
	LockOnSystem *lockon_sys_;

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

	void FireMissile(const MissileArgs &args);
	void Move(float speed);
	void UpdateCollision();
};
\
