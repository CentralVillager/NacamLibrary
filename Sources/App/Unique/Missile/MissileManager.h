#pragma once
#include <vector>
#include <list>
#include <DirectXMath.h>
#include "Missile.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"

/// <summary>
/// ミサイルのコンテナを管理します。
/// </summary>
class MissileManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:

	std::list<Missile> missile_list_;

	LockOnSystem *p_lockon_sys_;

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
	LockOnSystem *GetLockOnSys() { return p_lockon_sys_; }

	void HomingTarget(EnemiesList &enemies);
	void Death(UINT n);
	void AddMissile(const MissileParam &args);
};
