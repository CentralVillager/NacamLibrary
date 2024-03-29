#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Enemy.h"
#include "../../Lib/Collision/CollisionPrimitive.h"
#include "../Number/Numbers.h"

class Player;

class EnemiesList
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	static const int MAX_ENEMY_NUM_ = 20;

private:

	std::vector<Enemy> enemies_;
	Player *player_;

	std::unique_ptr<Numbers> nums_;

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
	void DrawRemainEnemyNum();
	void DebugDraw();
	void DrawColl();

public:

	/// <summary>
	/// 敵の生成
	/// </summary>
	/// <param name="pos"></param>
	void Add(const XMFLOAT3 &pos);

	/// <summary>
	/// 決まった敵セットの生成
	/// </summary>
	void AddTemplateSet();

	void Death(int i) { enemies_[i].Death(); }
	bool NoticeEmpty();

	const int32_t GetEnemyIndexWithID(int32_t id);
};
