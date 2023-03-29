#pragma once
#include <memory>
#include "../Abs/AbsUniqueObj.h"

class Player;
class MissileLauncher;

/// <summary>
/// 敵
/// </summary>
class Enemy : public AbsUniqueObj
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// ミサイル発射間隔
	static constexpr uint32_t LAUNCH_MISSILE_INTERVAL_ = 300;

private:

	// モデルデータ
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	// ID付与用カウンター
	static int id_counter_;

	static Player *player_;

private:

	// ミサイル
	std::shared_ptr<MissileLauncher> mi_launcher_;

	// 個体判別用ID
	uint32_t id_;

	// 反転間隔
	int count_;

	// 円運動用アングル
	float circular_angle_;

	// 発射間隔保存用
	uint32_t shot_interval_;

	// ミサイル発射間隔保存用
	uint32_t missile_launch_count_;

	int32_t cycle = 100;
	float length = 100.0f;

public:

	Enemy();
	~Enemy();

public:

	static void LoadResources();
	static void ImportPtr(Player *player);

public:

	const uint32_t &GetID() { return id_; }

public:

	void Initialize(const XMFLOAT3 &pos);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// y軸回転を行う
	/// </summary>
	void RotY();

	/// <summary>
	/// 水平方向に移動
	/// </summary>
	void MoveHorizontally(const float speed, const float range);

	/// <summary>
	/// z軸方向へ移動
	/// </summary>
	/// <param name="speed"></param>
	void MoveZ(const float speed);

	/// <summary>
	/// 円運動を行う
	/// </summary>
	void MoveCircular();

	/// <summary>
	/// ミサイルを単発発射する
	/// </summary>
	void FireMissile();

	/// <summary>
	/// ミサイル発射関連の動作
	/// </summary>
	void LaunchMissileSequence();

private:

	/// <summary>
	/// ミサイルを発射できるか
	/// </summary>
	/// <returns></returns>
	bool CheckCanFire();
};
