#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Enemy.h"
#include "../Collision/CollisionPrimitive.h"

class EnemiesList
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	static const int MAX_ENEMY_NUM_ = 20;

	std::vector<Enemy> enemies_;

public:

	EnemiesList();
	~EnemiesList();

public:

	std::vector<Enemy> &GetEnemies() { return enemies_; }
	const XMFLOAT3 &GetPos(UINT i) { return enemies_[i].GetPos(); }
	const Sphere &GetCollData(UINT i) { return enemies_[i].GetCollData(); }
	const bool &GetIsDead(UINT i) { return enemies_[i].GetIsDead(); }
	const size_t &GetSize() { return enemies_.size(); }
	const int &GetID(UINT i) { return enemies_[i].GetID(); }

	void Update();
	void Draw();
	void DebugDraw();
	void DrawColl();

public:

	void Add(const XMFLOAT3 &pos);
	void AddTemplateSet();
	void Death(int i) { enemies_[i].Death(); }
	bool NoticeEmpty();

	const int &GetEnemyIndexWithID(UINT id);
};
