#pragma once
#include "../Sources/App/Unique/Bullet/Bullet.h"
#include "../Bullet/BulletList.h"
#include "../Abs/AbsUniqueObj.h"

class Player;

class Enemy : public AbsUniqueObj
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// モデルデータ
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	static int id_counter_;
	static Player *player_;

	int id_;

	int count_;

	float circular_angle_;

	std::shared_ptr<BulletList> bullets_;
	int shot_interval_;

public:

	Enemy();
	~Enemy();

public:

	static void LoadResources();
	static void ImportPtr(Player *player);

	const int &GetID() { return id_; }

	void Initialize(const XMFLOAT3 &pos);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	void RotY();
	void MoveHorizontally(const float &speed, const float &range);
	void MoveCircular();

	void AutoShot(int interval, const XMFLOAT3 &dist);
};