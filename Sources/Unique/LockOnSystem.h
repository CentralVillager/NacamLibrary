#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Model/Model.h"
#include "../Object3d/Object3d.h"
#include "../Unique/EnemiesList.h"

class Player;

class LockOnSystem
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	static std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> marker_;

	XMFLOAT3 target_pos_;
	int tgt_index_;

	Player *player_ptr_;
	EnemiesList *enemies_ptr_;

public:

	LockOnSystem();
	~LockOnSystem();

public:

	static void LoadResources();

	void Initialize(Player *player, EnemiesList *enemies_ptr);
	void Update();
	void Draw();

public:

	const XMFLOAT3 &GetTgtPos() { return target_pos_; }
	const int &GetTgtIndex() { return tgt_index_; }

private:

	void CalcNearestTarget(const XMFLOAT3 &player_pos, EnemiesList &enemies);
	float CalcDistance(XMFLOAT3 dist_pos, XMFLOAT3 src_pos);
};
