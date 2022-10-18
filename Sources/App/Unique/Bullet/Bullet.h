#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Sources/Lib/Object3d/Object3d.h"
#include "../Sources/Lib/Model/Model.h"
#include "../Sources/App/Collision/CollisionPrimitive.h"

class Bullet
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> sphere_model_;
	std::unique_ptr<Object3d> object_;
	std::unique_ptr<Object3d> sphere_obj_;
	Sphere coll_;
	float COLL_RADIUS_ = 2.0f;

	static constexpr float SPEED_ = 3.0f;

	XMFLOAT3 pos_{};
	XMFLOAT3 vel_{};
	int life_ = 50;
	bool is_dead_ = true;

public:

	Bullet();
	~Bullet();

public:

	static void LoadResources();

public:

	void Initialize(const XMFLOAT3 &pos);
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	void Fire(const XMFLOAT3 &dist);
	void CalcAngle(const XMFLOAT3 &dist);


private:

	void UpdateCollision();
};
