#pragma once
#include <vector>
#include <forward_list>
#include <DirectXMath.h>
#include "Missile.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Unique/EnemiesList.h"
#include "../Unique/LockOnSystem.h"

class MissileManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	std::forward_list<Missile> missile_list_;

	LockOnSystem *lockon_sys_;

public:

	MissileManager();
	~MissileManager();

public:

	void Initialize(LockOnSystem *sys);
	void Finalize();
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	void Fire(const MissileArgs &args);
	void FireMultiMissile(const MissileArgs &args);
	void HomingTarget(XMFLOAT3 target_pos);
	void HomingTarget(EnemiesList &enemies);

	bool CalcCollision(const Sphere &enemy);

private:

	void AddMissile(const MissileArgs &args);
};
