#pragma once
#include <memory>
#include "../Sources/Lib/Object3d/Object3d.h"
#include "../Sources/Lib/Model/Model.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../Sources/App/Unique/Bullet/Bullet.h"

class Player;

class Enemy
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> sphere_model_;
	std::shared_ptr<Object3d> object_;
	std::shared_ptr<Object3d> sphere_obj_;
	Sphere coll_;
	float COLL_RADIUS_ = 2.0f;

	bool is_dead_ = false;
	static int id_counter_;
	int id_;

	float speed_ = 0.5f;
	int count_ = 100;

	float circular_angle_ = 0.0f;

	std::shared_ptr<Bullet> bullet_;
	int shot_interval_ = 0;

	static Player *player_;

public:

	Enemy();
	~Enemy();

public:

	static void LoadResources();
	static void ImportPtr(Player *player);

	const XMFLOAT3 &GetPos() { return object_->GetPosition(); }
	const Sphere &GetCollData() { return coll_; }
	const bool &GetIsDead() { return is_dead_; }
	const int &GetID() { return id_; }

	void Death() { is_dead_ = true; }

	void Initialize(const XMFLOAT3 &pos);
	void Finalize();
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	void RotY();
	void MoveHorizontally(const float &speed, const float &range);
	void MoveCircular();

	void AutoShot(int interval, const XMFLOAT3 &pos);

private:

	void UpdateCollision();
};
