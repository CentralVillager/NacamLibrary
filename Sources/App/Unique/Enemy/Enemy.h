#pragma once
#include "../Sources/App/Unique/Bullet/Bullet.h"
#include "../Bullet/BulletList.h"
#include "../Abs/AbsUniqueObj.h"

class Player;

class Enemy : public AbsUniqueObj
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// ƒ‚ƒfƒ‹ƒf[ƒ^
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	static int id_counter_;
	static Player *player_;

	uint32_t id_;

	int count_;

	float circular_angle_;

	std::shared_ptr<BulletList> bullets_;
	int shot_interval_;

	int32_t cycle = 100;
	float length = 100.0f;

public:

	Enemy();
	~Enemy();

public:

	static void LoadResources();
	static void ImportPtr(Player *player);

	const uint32_t &GetID() { return id_; }
	const std::shared_ptr<BulletList> &GetBulletList() { return bullets_; }

	void Initialize(const XMFLOAT3 &pos);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// y²‰ñ“]‚ğs‚¤
	/// </summary>
	void RotY();

	/// <summary>
	/// …•½•ûŒü‚ÉˆÚ“®
	/// </summary>
	void MoveHorizontally(const float &speed, const float &range);

	/// <summary>
	/// ‰~‰^“®‚ğs‚¤
	/// </summary>
	void MoveCircular();

	/// <summary>
	/// İ’è‚µ‚½ŠÔŠu‚Å’e‚ğ”­Ë‚·‚é
	/// </summary>
	void AutoShot(int interval, const XMFLOAT3 &dist);
};