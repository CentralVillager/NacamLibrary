#pragma once
#include <memory>
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
#include "../Sources/Lib/Model/Model.h"
#include "../Sources/Lib/Object3d/Object3d.h"
#include "../Enemy/EnemiesList.h"
#include "../Sources/App/Number/Numbers.h"

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
		UINT id;
	};

private:

	// 現在のロックオン可能最大数
	static UINT current_tgt_num_;

private:

	static std::unique_ptr<Model> model_;
	std::vector<Object3d> markers_;

	UINT multi_target_num_;

	std::vector<TargetData> tgt_datas_;

	Player *player_ptr_;
	EnemiesList *enemies_ptr_;

	std::unique_ptr<Numbers> numbers_;

public:

	LockOnSystem();
	~LockOnSystem();

public:

	static void LoadResources();

	void Initialize(Player *player, EnemiesList *enemies_ptr);
	void Update();
	void Draw();
	void DrawNum();
	void DebugDraw();

public:

	inline const TargetData &GetTgtData(int i) { return tgt_datas_[i]; }
	inline const std::vector<TargetData> &GetTgtData() { return tgt_datas_; }

public:

	void AddTargetNum();
	void ResetTargetNum();

private:

	void CalcNearestTargets(const XMFLOAT3 &player_pos, EnemiesList &enemies);
	float CalcDistance(const XMFLOAT3 &dist_pos, const XMFLOAT3 &src_pos);
};
