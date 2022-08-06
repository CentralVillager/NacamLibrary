#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Object3d/Object3d.h"
#include "../Model/Model.h"

struct MissileArgs {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT3 acc;
	int life;
	bool is_alive;
};

class Missile {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	static std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> object_;

	MissileArgs args_;

public:

	Missile();
	~Missile();

public:

	void Initialize(const MissileArgs &args);
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

public:

	const bool &GetIsAlive() { return args_.is_alive; }

	void MoveZ(float speed);
	void FollowingTarget(XMFLOAT3 target_pos);
	const void Activate() { args_.is_alive = true; }
};
