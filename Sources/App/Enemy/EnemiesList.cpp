#include "EnemiesList.h"
#include <algorithm>
#include "../Sources/Lib/NacamError/NacamError.h"
#include "../Player/Player.h"
#include "../Missile/MissileDescs.h"

EnemiesList::EnemiesList() :
	enemies_(),
	player_(nullptr),
	nums_(std::make_unique<Numbers>())
{}

EnemiesList::~EnemiesList()
{}

void EnemiesList::Initialize(Player *player)
{
	player_ = player;
	Enemy::ImportPtr(player);
}

void EnemiesList::Update()
{
	for (auto &i : enemies_)
	{
		i.Update();
	}

	std::erase_if(enemies_, [](Enemy &x) { return x.IsDead(); });
}

void EnemiesList::Draw()
{
	for (auto &i : enemies_)
	{
		if (!i.IsDead())
		{
			i.Draw();
		}
	}
}

void EnemiesList::DrawRemainEnemyNum()
{
	nums_->DrawNumber((int)(enemies_.size()), 0.5f, 1.0f, XMFLOAT2(0.0f, 0.0f), HorizontalAlignment::Left);
}

void EnemiesList::DebugDraw()
{
	if (ImGui::Button("Launch Missile"))
	{
		for (auto &i : enemies_)
		{
			i.FireMissile();
		}
	}
}

void EnemiesList::DrawColl()
{
	for (auto &i : enemies_)
	{
		if (!i.IsDead())
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
	UINT ene_num = 20;

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

const int32_t EnemiesList::GetEnemyIndexWithID(int32_t id)
{
	// ターゲットがプレイヤーなら
	if (id == (int32_t)(TargetIs::Player))
	{
		// そう返す
		return (int32_t)(TargetIs::Player);
	}

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
	return (int32_t)(NacamError::NonDetected);
}
