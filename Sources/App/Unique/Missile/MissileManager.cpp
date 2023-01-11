#include "MissileManager.h"
#include "../Sources/App/Collision/Collision.h"
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Player/Player.h"

using namespace DirectX;
using namespace NcmUtill;

MissileManager::MissileManager() :
	missile_list_(),
	p_lockon_sys_(nullptr)
{}

MissileManager::~MissileManager()
{}

void MissileManager::Initialize(Player *p, LockOnSystem *sys)
{
	p_player_ = p;
	p_lockon_sys_ = sys;
}

void MissileManager::Finalize()
{}

void MissileManager::Update()
{
	for (auto &i : missile_list_)
	{
		i.Update();
	}

	missile_list_.remove_if([](Missile &x) { return x.IsDead(); });
}

void MissileManager::Draw()
{
	for (auto &i : missile_list_)
	{
		i.Draw();
	}
}

void MissileManager::DrawColl()
{
	for (auto &i : missile_list_)
	{
		i.DrawColl();
	}
}

void MissileManager::DebugDraw()
{
	for (auto &i : missile_list_)
	{
		//i.DebugDraw();
	}
}

const bool &MissileManager::GetIsValidity(UINT n)
{
	auto itr = MoveIterator(missile_list_.begin(), n);
	return itr->GetIsValidity();
}

const Sphere &MissileManager::GetCollData(UINT n)
{
	auto itr = MoveIterator(missile_list_.begin(), n);
	return itr->GetCollData();
}

int32_t MissileManager::GetTgtID(int32_t n)
{
	auto itr = MoveIterator(missile_list_.begin(), n);
	return itr->GetMissileParam().tgt_id;
}

void MissileManager::HomingEnemy(EnemiesList &enemies)
{
	for (auto &i : missile_list_)
	{
		i.HomingEnemy(enemies);
	}
}

void MissileManager::HomingTarget()
{
	for (auto &i : missile_list_)
	{
		if (i.GetIsValidity())
		{
			i.HomingTarget();
		}
	}
}

void MissileManager::UpdateTargetPos(EnemiesList &enemies)
{
	for (auto &i : missile_list_)
	{
		if (i.GetIsValidity())
		{
			i.UpdateTargetPos(enemies);
		}
	}
}

bool MissileManager::Death(UINT n)
{
	// 死んだミサイルを特定
	auto itr = MoveIterator(missile_list_.begin(), n);

	// エミッターの終了準備
	itr->PrepareTermEmitter();
	// ミサイルの寿命を強制的に0に
	itr->SetMissileLife(0);
	// ミサイルを無効化(死亡フラグは建てない)
	itr->InvalidateMissile();

	return true;
}

void MissileManager::AddMissile(const MissileParam &param)
{
	missile_list_.emplace_front();
	missile_list_.front().Initialize(param);
	missile_list_.front().Activate();
}
