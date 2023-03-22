#pragma once
#include <memory>
#include <DirectXMath.h>
#include <vector>
#include <list>
#include <algorithm>
#include "../../../Lib/3D/Model.h"
#include "../../../Lib/3D/Object3d.h"
#include "../Enemy/EnemiesList.h"
#include "../Sources/App/Number/Numbers.h"
#include "../../Utility/NcmUtil.h"

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
		uint32_t id;
	};

public:

	// デフォルトの最大同時ロックオン可能数
	static constexpr uint32_t DEFAULT_TGT_NUM_ = 4;

private:

	// 現在のロックオン可能最大数
	static uint32_t current_tgt_num_;
	static uint32_t max_tgt_num_;

private:

	// 描画データ
	static std::unique_ptr<Model> model_;
	std::vector<Object3d> markers_;

	// 距離に応じて昇順にソートされたターゲットのリスト
	std::list<TargetData> near_tgt_list_;

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

	inline const std::list<TargetData> &GetTgtList() { return near_tgt_list_; }
	inline static const uint32_t &GetCurrentTgtNum() { return current_tgt_num_; }
	const uint32_t GetMaxTgtNum();

	inline static const void SetMaxTgtNum(uint32_t n) { max_tgt_num_ = n; }

public:

	/// <summary>
	/// ターゲット数を増やす
	/// </summary>
	void AddTargetNum();

	/// <summary>
	/// ターゲット数をリセットする
	/// </summary>
	void ResetTargetNum();

private:

	/// <summary>
	/// 一番近い敵を算出する
	/// </summary>
	void CalcNearestTargets(const XMFLOAT3 &player_pos, EnemiesList &enemies);

	/// <summary>
	/// 距離を算出する
	/// </summary>
	float CalcDistance(const XMFLOAT3 &dist_pos, const XMFLOAT3 &src_pos);
};
