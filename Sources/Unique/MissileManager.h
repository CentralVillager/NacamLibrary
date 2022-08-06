#pragma once
#include <forward_list>
#include <DirectXMath.h>
#include "Missile.h"

class MissileManager {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::forward_list<Missile> missile_list_;

public:

	MissileManager();
	~MissileManager();

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

public:

	void Fire();
	void FollowingTarget(XMFLOAT3 target_pos);

private:

	void AddMissile();
};
