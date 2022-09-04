#pragma once
#include "../Object3d/Object3d.h"
#include "../Model/Model.h"
#include "../Collision/CollisionPrimitive.h"

class Enemy
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> sphere_model_;
	std::shared_ptr<Object3d> object_;
	std::shared_ptr<Object3d> sphere_obj_;
	Sphere coll_;
	const float COLL_RADIUS_ = 2.0f;

	bool is_dead_ = false;

	float speed_ = 0.5f;
	int count_ = 100;

public:

	Enemy();
	~Enemy();

public:

	const XMFLOAT3 &GetPos() { return object_->GetPosition(); }
	const Sphere &GetCollData() { return coll_; }
	const bool &GetIsDead() { return is_dead_; }

	static void LoadResources();
	void Death() { is_dead_ = true; }

	void Initialize(XMFLOAT3 pos);
	void Finalize();
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	void RotY();
	void MoveHorizontally(float speed, float range);

private:

	void UpdateCollision();
};
