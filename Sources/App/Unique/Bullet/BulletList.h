#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Bullet.h"

class BulletList
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::vector<Bullet> bullets_;
	XMFLOAT3 init_pos_;

public:

	BulletList();
	~BulletList();

public:

	inline void SetPos(const XMFLOAT3 &pos) { init_pos_ = pos; }

	void Initialize();
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	void Fire(const XMFLOAT3 &src, const XMFLOAT3 &dist);
	void CalcAngle(const XMFLOAT3 &dist);
};
