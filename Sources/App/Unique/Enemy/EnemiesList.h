#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Enemy.h"
#include "../../Collision/CollisionPrimitive.h"

class Player;

class EnemiesList
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	static const int MAX_ENEMY_NUM_ = 20;

	std::vector<Enemy> enemies_;
	Player *player_;

public:

	EnemiesList();
	~EnemiesList();

public:

	std::vector<Enemy> &GetEnemies() { return enemies_; }
	const XMFLOAT3 &GetPos(UINT i) { return enemies_[i].GetPos(); }
	const Sphere &GetCollData(UINT i) { return enemies_[i].GetCollData(); }
	const bool &GetIsDead(UINT i) { return enemies_[i].IsDead(); }
	const size_t &GetSize() { return enemies_.size(); }
	const uint32_t GetID(UINT i) { return enemies_[i].GetID(); }

	void Initialize(Player *player);
	void Update();
	void Draw();
	void DebugDraw();
	void DrawColl();

public:

	void Add(const XMFLOAT3 &pos);
	void AddTemplateSet();
	void Death(int i) { enemies_[i].Death(); }
	bool NoticeEmpty();

	const uint32_t GetEnemyIndexWithID(uint32_t id);
};
