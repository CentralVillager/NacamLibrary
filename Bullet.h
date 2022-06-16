#pragma once
#include <DirectXMath.h>
#include "Model.h"
#include "Object3d.h"
#include <memory>
#include "Collision.h"
#include "AudioManager.h"

class Bullet {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// 当たり判定
	AABB collision = {};

	// 弾モデル
	static std::unique_ptr<Model> model_bullet_;

	// 弾
	std::shared_ptr<Object3d> bullet_ = nullptr;

	// 速度
	float speed_;

	// 寿命
	int life_ = 0;

	// 生存しているか
	bool is_alive_ = false;

public:
	static void InitializeModel();

	void Initialize(float speed, int life);
	void Finalize();
	void Update();
	void Draw();

	const bool &GetIsAlive() { return is_alive_; }
	const AABB &GetAABB() { return collision; }
	void SetIsAlive(bool is_alive) { is_alive_ = is_alive; }

	/// <summary>
	/// 弾を生成する
	/// </summary>
	void Generate(XMFLOAT3 pos, float speed, int life);

	/// <summary>
	/// 弾を前方に飛ばす
	/// </summary>
	void MoveForward();

	/// <summary>
	/// 当たり判定の位置を更新する
	/// </summary>
	void UpdateAABB();
};
