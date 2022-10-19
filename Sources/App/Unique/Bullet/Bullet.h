#pragma once
#include "../Abs/AbsUniqueObj.h"

class Bullet : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	XMFLOAT3 vel_{};
	int life_ = 50;

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

	void Fire(const XMFLOAT3 &dist);
	void CalcAngle(const XMFLOAT3 &dist);
};