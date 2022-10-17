#pragma once
#include <vector>
#include <list>
#include <DirectXMath.h>
#include "Missile.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"

class MissileManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	std::list<Missile> missile_list_;

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

	inline const std::list<Missile> &GetMissileList() { return missile_list_; }
	const bool &GetIsValidity(UINT n);
	const Sphere &GetCollData(UINT n);

	void Fire(const MissileArgs &args);
	void FireMultiMissile(const MissileArgs &args);
	void HomingTarget(XMFLOAT3 target_pos);
	void HomingTarget(EnemiesList &enemies);
	void Death(UINT n);
	bool CalcCollision(const Sphere &enemy);

private:

	void AddMissile(const MissileArgs &args);
};
