#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Model.h"
#include "Object3d.h"
#include "Collision.h"
#include "AudioManager.h"

enum class Pattern {
	NUTORAL,
	HORIZONTAL,
	VERTICAL
};

class Enemy {
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	// 方向
	enum class Direction {
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

private:

	// 当たり判定
	AABB collision = {};

	// 敵モデル
	static std::unique_ptr<Model> model_enemy_;

	// 敵オブジェクト
	std::shared_ptr<Object3d> enemy_ = nullptr;

	// スピード
	float move_speed_ = 0;

	// 移動可能範囲
	static float movable_range_[4];

	// 生存しているか
	bool is_alive_ = false;

	std::shared_ptr<AudioManager> death_;

	Pattern pattern_;

public:

	// 現在出現している敵の数
	static int spawned_enemy_;

	static void InitializeModel();

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	const AABB &GetAABB() { return collision; }
	void SetIsAlive(bool is_alive) { is_alive_ = is_alive; }

	void Spawn(XMFLOAT3 pos, Pattern p);

	void Move();

	// 左右に反復移動
	void MoveHorizontally();

	// 上下に反復移動
	void MoveVertically();

	/// <summary>
	/// 当たり判定の位置を更新する
	/// </summary>
	void UpdateAABB();
};
