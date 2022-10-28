#include "MissileManager.h"
#include "../Sources/App/Collision/Collision.h"
#include "../Sources/App/Utility/NcmUtil.h"

using namespace DirectX;
using namespace NcmUtill;

MissileManager::MissileManager() :
	missile_list_(),
	p_lockon_sys_(nullptr)
{}

MissileManager::~MissileManager()
{}

void MissileManager::Initialize(LockOnSystem *sys)
{
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
		i.DebugDraw();
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

void MissileManager::Fire(const MissileArgs &args)
{
	AddMissile(args);
}

void MissileManager::FireMultiMissile(const MissileArgs &args, uint32_t num)
{
	MissileArgs l_args{};
	l_args = args;

	// ロック上限数まで
	for (UINT i = 0; i < num; i++)
	{
		// 加速度をランダムに設定
		//XMFLOAT3 rand = GenerateRandom(XMFLOAT3(-1.0f, -1.0f, 0), XMFLOAT3(1.0f, 1.0f, 0));
		XMFLOAT3 rand = { 0, -1.0f, 0 };
		l_args.acc = rand;

		auto itr = MoveIterator(p_lockon_sys_->GetTgtList().begin(), i);

		// ターゲットデータを取得
		l_args.tgt_pos = itr->pos;
		l_args.tgt_id = itr->id;

		// ミサイルを追加
		AddMissile(l_args);
	}
}

void MissileManager::FireChargeMissile(const MissileArgs &args)
{
	MissileArgs l_args{};
	l_args = args;

	// ロック上限数まで
	for (UINT i = 0; i < p_lockon_sys_->GetCurrentTgtNum(); i++)
	{
		// 加速度をランダムに設定
		//XMFLOAT3 rand = GenerateRandom(XMFLOAT3(-1.0f, -1.0f, 0), XMFLOAT3(1.0f, 1.0f, 0));
		XMFLOAT3 rand = { 0, -1.0f, 0 };
		l_args.acc = rand;

		auto itr = MoveIterator(p_lockon_sys_->GetTgtList().begin(), i);

		// ターゲットデータを取得
		l_args.tgt_pos = itr->pos;
		l_args.tgt_id = itr->id;

		// ミサイルを追加
		AddMissile(l_args);
	}
}

void MissileManager::FireUltimateMissile(const MissileArgs &args, uint32_t launched)
{
	MissileArgs l_args{};
	l_args = args;

	p_lockon_sys_->SetMaxTgtNum(10);

	// 加速度をランダムに設定
	XMFLOAT3 rand = GenerateRandom(XMFLOAT3(-1.0f, -1.0f, 0), XMFLOAT3(1.0f, 1.0f, 0));
	//XMFLOAT3 rand = { 0, -1.0f, 0 };
	l_args.acc = rand;

	if (p_lockon_sys_->GetMaxTgtNum() < launched)
	{
		launched = p_lockon_sys_->GetMaxTgtNum() - 1;
	}

	auto itr = MoveIterator(p_lockon_sys_->GetTgtList().begin(), launched);

	// ターゲットデータを取得
	l_args.tgt_pos = itr->pos;
	l_args.tgt_id = itr->id;

	// ミサイルを追加
	AddMissile(l_args);

	p_lockon_sys_->SetMaxTgtNum(4);
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

void MissileManager::Death(UINT n)
{
	auto itr = MoveIterator(missile_list_.begin(), n);

	itr->TermEmitter();
	itr->SetMissileLife(0);
	itr->InvalidateMissile();
}

void MissileManager::AddMissile(const MissileArgs &args)
{
	missile_list_.emplace_front();
	missile_list_.front().Initialize(args, p_lockon_sys_);
	missile_list_.front().Activate();
}
