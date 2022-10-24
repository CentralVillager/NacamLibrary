#pragma once
#include "../Abs/AbsUniqueObj.h"

class Bullet : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	// モデルデータ
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	XMFLOAT3 vel_{};
	int life_;
	float angle_;

public:

	Bullet();
	~Bullet();

public:

	static void LoadResources();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	void Fire(const XMFLOAT3 &src, const XMFLOAT3 &dist);
	void CalcVelocity(const XMFLOAT3 &dist);
};
