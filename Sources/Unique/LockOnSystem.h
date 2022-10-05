#pragma once
#include <memory>
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
#include "../Model/Model.h"
#include "../Object3d/Object3d.h"
#include "../Unique/EnemiesList.h"

class Player;

class LockOnSystem
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	struct TargetData
	{
		float dist;
		XMFLOAT3 pos;
		int index;
	};

private:

	static std::unique_ptr<Model> model_;
	std::vector<Object3d> markers_;

	int multi_target_num_;

	std::vector<TargetData> tgt_datas_;
	int tgt_index_;

	Player *player_ptr_;
	EnemiesList *enemies_ptr_;

public:

	LockOnSystem();
	~LockOnSystem();

public:

	static void LoadResources();

	void Initialize(Player *player, EnemiesList *enemies_ptr, int multi_tgt_n);
	void Update();
	void Draw();

public:

	const TargetData &GetTgtData(int i) { return tgt_datas_[i]; }
	const size_t &GetTgtDataSize() { return tgt_datas_.size(); }
	const int &GetTgtIndex() { return tgt_index_; }

private:

	void CalcNearestTargets(const XMFLOAT3 &player_pos, EnemiesList &enemies);
	float CalcDistance(XMFLOAT3 dist_pos, XMFLOAT3 src_pos);
};
