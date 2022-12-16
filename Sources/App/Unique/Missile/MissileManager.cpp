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

void MissileManager::HomingTarget(EnemiesList &enemies)
{
	for (auto &i : missile_list_)
	{
		i.HomingTarget(enemies);
		//i.TestHomingTarget(enemies);
	}
}

void MissileManager::Death(UINT n)
{
	// ���񂾃~�T�C�������
	auto itr = MoveIterator(missile_list_.begin(), n);

	// �G�~�b�^�[�̏I������
	itr->PrepareTermEmitter();
	// �~�T�C���̎����������I��0��
	itr->SetMissileLife(0);
	// �~�T�C���𖳌���(���S�t���O�͌��ĂȂ�)
	itr->InvalidateMissile();
}

void MissileManager::AddMissile(const MissileParam &args)
{
	missile_list_.emplace_front();
	missile_list_.front().Initialize(args);
	missile_list_.front().Activate();
}
