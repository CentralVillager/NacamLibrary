#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Model.h"
#include "Object3d.h"
#include "Bullet.h"
#include <vector>
#include "Collision.h"

class Player {
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

	// プレイヤーモデル
	std::shared_ptr<Model> model_fighter_ = nullptr;

	// プレイヤーオブジェクト
	std::unique_ptr<Object3d> player_ = nullptr;

	// スピード
	float move_speed_ = 0;

	// 弾速
	float bullet_speed_ = 0;

	// 弾の連射速度
	int rapid_fire_speed_ = 0;

	// 弾の寿命
	int bullet_life_ = 0;

	// HP
	std::unique_ptr<uint16_t> hit_point_ = 0;

	// 移動可能範囲
	float movable_range_[4] = {};

	// 弾
	std::vector<Bullet> bullet_ = {};

	std::unique_ptr<AudioManager> shot_;

public:

	Player() {};
	~Player() {};

	const vector<Bullet> &GetBulletVector() { return bullet_; }
	Bullet &GetBullet(int num) { return bullet_[num]; }
	const AABB &GetAABB() { return collision; }

	void Initialize(float speed, float bullet_speed, int rapid_fire_speed);
	void Finalize();
	void Update();
	void Draw();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 弾の発射
	/// </summary>
	void Shot();

	/// <summary>
	/// 寿命が尽きた弾をvector配列から削除する
	/// </summary>
	void DeleteDeadBullet();

	/// <summary>
	/// 当たり判定の位置を更新する
	/// </summary>
	void UpdateAABB();
};
