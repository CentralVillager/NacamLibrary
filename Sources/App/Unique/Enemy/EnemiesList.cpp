#include "EnemiesList.h"
#include <algorithm>
#include "../Sources/Lib/NacamError/NacamError.h"
#include "../Player/Player.h"

using namespace std;

EnemiesList::EnemiesList()
{
	enemies_.resize(0);
}

EnemiesList::~EnemiesList()
{}

void EnemiesList::Initialize(Player *player)
{
	player_ = player;
	Enemy::ImportPtr(player);
}

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
{
	for (auto &i : enemies_)
	{
		i.DebugDraw();
	}
}

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

void EnemiesList::Add(const XMFLOAT3 &pos)
{
	enemies_.emplace_back();
	enemies_.back().Initialize(pos);
}

void EnemiesList::AddTemplateSet()
{
	float offset = 10.0f;
	float z_offset = 50.0f;
	UINT ene_num = 10;

	Add(XMFLOAT3(0, 0, z_offset));
	for (UINT i = 0; i < ene_num / 2; i++)
	{
		Add(XMFLOAT3(offset * i + offset, 0, z_offset * i + z_offset));
	}
	for (UINT i = 0; i < ene_num / 2; i++)
	{
		Add(XMFLOAT3(-(offset * i + offset), 0, z_offset * i + z_offset));
	}
}

bool EnemiesList::NoticeEmpty()
{
	if (enemies_.empty())
	{
		return true;
	}

	return false;
}

const int &EnemiesList::GetEnemyIndexWithID(UINT id)
{
	// 全ての敵に対してIDを検索
	for (UINT i = 0; i < enemies_.size(); i++)
	{
		// 該当IDが見つかったら
		if (enemies_[i].GetID() == id)
		{
			// そのIDを持つ敵の添字を返す
			return i;
		}
	}

	// 該当IDが存在しないのでエラーを示させる
	return (int)(NacamError::NonDetected);
}
