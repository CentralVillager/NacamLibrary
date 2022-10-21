#include "BulletList.h"

BulletList::BulletList() :
	bullets_(),
	init_pos_()
{}

BulletList::~BulletList()
{}

void BulletList::Initialize()
{
	for (auto &i : bullets_)
	{
		i.Initialize();
	}
}

void BulletList::Update()
{
	for (auto &i : bullets_)
	{
		i.Update();
	}

	std::erase_if(bullets_, [](Bullet &x) { return x.IsDead(); });
}

void BulletList::Draw()
{
	for (auto &i : bullets_)
	{
		i.Draw();
	}
}

void BulletList::DrawColl()
{
	for (auto &i : bullets_)
	{
		i.DrawColl();
	}
}

void BulletList::DebugDraw()
{}

void BulletList::Fire(const XMFLOAT3 &src, const XMFLOAT3 &dist)
{
	bullets_.emplace_back();
	bullets_.back().Initialize();
	bullets_.back().Fire(src, dist);
}

void BulletList::CalcAngle(const XMFLOAT3 &dist)
{
	for (auto &i : bullets_)
	{
		i.CalcAngle(dist);
	}
}
