#include "MissileManager.h"
#include "../Collision/Collision.h"

using namespace DirectX;

MissileManager::MissileManager()
{
	missile_list_.resize(0);
}

MissileManager::~MissileManager()
{}

void MissileManager::Initialize(LockOnSystem *sys)
{
	lockon_sys_ = sys;
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
{}

void MissileManager::Fire(const MissileArgs &args)
{
	AddMissile(args);
}

void MissileManager::FireMultiMissile(const MissileArgs &args)
{
	MissileArgs l_args{};
	l_args = args;

	// ロック上限数まで
	for (UINT i = 0; i < lockon_sys_->GetTgtData().size(); i++)
		//for (UINT i = 0; i < lockon_sys_->GetTgtDataSize(); i++)
	{
		// ターゲットデータを取得
		l_args.tgt_pos = lockon_sys_->GetTgtData(i).pos;
		l_args.tgt_index = lockon_sys_->GetTgtData(i).index;

		// ミサイルを追加
		AddMissile(l_args);
	}
}

void MissileManager::HomingTarget(XMFLOAT3 target_pos)
{
	for (auto &i : missile_list_)
	{
		i.HomingTarget(target_pos);
	}
}

void MissileManager::HomingTarget(EnemiesList &enemies)
{
	for (auto &i : missile_list_)
	{
		i.HomingTarget(enemies);
	}
}

bool MissileManager::CalcCollision(const Sphere &enemy)
{
	for (auto &i : missile_list_)
	{
		/*if (i.GetIsHit())
		{
			return false;
		}*/

		if (Collision::CheckSphere2Sphere(i.GetCollData(), enemy))
		{
			i.TermEmitter();
			i.SetMissileLife(0);

			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void MissileManager::AddMissile(const MissileArgs &args)
{
	missile_list_.emplace_front();
	missile_list_.front().Initialize(args, lockon_sys_);
	missile_list_.front().Activate();
}
