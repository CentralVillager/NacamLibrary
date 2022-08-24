#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Enemy.h"
#include "../Collision/CollisionPrimitive.h"

class EnemiesList
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::vector<Enemy> enemies_;

public:

	EnemiesList();
	~EnemiesList();

public:

	std::vector<Enemy> &GetEnemies() { return enemies_; }
	const XMFLOAT3 &GetPos(UINT i) { return enemies_[i].GetPos(); }
	const Sphere &GetCollData(UINT i) { return enemies_[i].GetCollData(); }
	const size_t &GetSize() { return enemies_.size(); }
	void Kill(int i) { enemies_[i].Kill(); } 

	void Update();
	void Draw();
	void DebugDraw();
	void DrawColl();

public:

	void Add(XMFLOAT3 pos);
};
