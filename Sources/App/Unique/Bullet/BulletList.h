#pragma once
#include <list>
#include <DirectXMath.h>
#include "Bullet.h"

class BulletList
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	//std::list<Bullet> bullets_;

public:

	BulletList();
	~BulletList();

public:

	void Update();
	void Draw();
	void DrawColl();

public:

	void Fire(const XMFLOAT3 &dist);
};
