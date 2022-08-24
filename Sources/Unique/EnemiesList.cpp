#include "EnemiesList.h"

using namespace std;

EnemiesList::EnemiesList()
{
	enemies_.resize(0);
}

EnemiesList::~EnemiesList()
{}

void EnemiesList::Update()
{
	/*for (auto &i : enemies_)
	{
		i.Update();

		if (i.GetIsDead())
		{

		}
	}*/

	for (UINT i = 0; i < enemies_.size(); i++)
	{
		enemies_[i].Update();

		if (enemies_[i].GetIsDead())
		{

		}
	}
}

void EnemiesList::Draw()
{
	for (auto &i : enemies_)
	{
		i.Draw();
	}
}

void EnemiesList::DebugDraw()
{}

void EnemiesList::DrawColl()
{
	for (auto &i : enemies_)
	{
		i.DrawColl();
	}
}

void EnemiesList::Add(XMFLOAT3 pos)
{
	enemies_.emplace_back();
	enemies_.back().Initialize(pos);
}
