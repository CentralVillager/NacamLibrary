#include "EnemiesList.h"
#include <algorithm>

using namespace std;

EnemiesList::EnemiesList()
{
	enemies_.resize(0);
}

EnemiesList::~EnemiesList()
{}

void EnemiesList::Update()
{
	for (UINT i = 0; i < enemies_.size(); i++)
	{
		enemies_[i].Update();

		if (enemies_[i].GetIsDead())
		{
			enemies_.erase(enemies_.begin() + i);
		}
	}
}

void EnemiesList::Draw()
{
	for (auto &i : enemies_)
	{
		if (!i.GetIsDead())
		{
			i.Draw();
		}
	}
}

void EnemiesList::DebugDraw()
{}

void EnemiesList::DrawColl()
{
	for (auto &i : enemies_)
	{
		if (!i.GetIsDead())
		{
			i.DrawColl();
		}
	}
}

void EnemiesList::Add(XMFLOAT3 pos)
{
	enemies_.emplace_back();
	enemies_.back().Initialize(pos);
}

bool EnemiesList::NoticeEmpty()
{
	if (enemies_.empty())
	{
		return true;
	}

	return false;
}
