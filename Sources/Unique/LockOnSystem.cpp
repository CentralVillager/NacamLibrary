#include "LockOnSystem.h"
#include "Player.h"
#include "../Debug/ImGuiManager.h"

using namespace DirectX;

UINT LockOnSystem::multi_tgt_num_ = 0;
std::unique_ptr<Model> LockOnSystem::model_ = nullptr;

LockOnSystem::LockOnSystem()
{
	tgt_datas_.resize(0);
	markers_.resize(0);
}

LockOnSystem::~LockOnSystem()
{}

void LockOnSystem::LoadResources()
{
	if (!model_)
	{
		model_ = std::make_unique<Model>();
		model_->LoadObjModel("Resources/ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void LockOnSystem::Initialize(Player *player, EnemiesList *enemies_ptr, UINT multi_tgt_n)
{
	player_ptr_ = player;
	enemies_ptr_ = enemies_ptr;

	multi_tgt_num_ = multi_tgt_n;
	multi_target_num_ = multi_tgt_n;
	for (UINT i = 0; i < multi_target_num_; i++)
	{
		AddTargetNum();
	}
}

void LockOnSystem::Update()
{
	CalcNearestTargets(player_ptr_->GetPos(), *enemies_ptr_);

	auto itr = tgt_datas_.begin();
	for (auto &i : markers_)
	{
		i.SetPosition({ itr->pos.x, itr->pos.y + 2.0f, itr->pos.z });
		i.Update();
		itr++;
	}
}

void LockOnSystem::Draw()
{
	for (auto &i : markers_)
	{
		i.Draw();
	}
}

void LockOnSystem::DebugDraw()
{
	ImGui::Text("target_num : %d", multi_target_num_);
}

void LockOnSystem::AddTargetNum()
{
	markers_.emplace_back();
	markers_.back().Initialize();
	markers_.back().SetModel(model_.get());
	markers_.back().SetScale(2.0f);

	tgt_datas_.emplace_back();
}

void LockOnSystem::ResetTargetNum()
{
	markers_.clear();
	tgt_datas_.clear();

	AddTargetNum();
}

void LockOnSystem::CalcNearestTargets(const XMFLOAT3 &player_pos, EnemiesList &enemies)
{
	// ��������
	float nearest_dist = 1000000.0f;

	// �G�����Ȃ��Ȃ�
	if (enemies.GetEnemies().size() <= 0)
	{
		// ���ʉ��Ƀ^�[�Q�b�g��u��
		tgt_datas_[0].pos = XMFLOAT3(0, 0, -100.0f);
	}

	// �\�[�g�p�R���e�i
	std::vector<TargetData> dist_sort;

	// �S�Ă̓G�ɑ΂���
	for (UINT i = 0; i < enemies.GetEnemies().size(); i++)
	{
		// �������v�Z�A�e�f�[�^���i�[
		dist_sort.emplace_back();
		dist_sort.back().dist = CalcDistance(player_pos, enemies.GetPos(i));
		dist_sort.back().pos = enemies.GetPos(i);
		dist_sort.back().id = enemies.GetID(i);
	}

	// dist�ɑ΂��ď����\�[�g
	std::sort(dist_sort.begin(), dist_sort.end(),
		[](const TargetData &a, const TargetData &b) { return a.dist < b.dist; });

	size_t size = 0;

	// ���b�N�I���\�� > �G�̑��� �Ȃ�
	if (tgt_datas_.size() > enemies.GetEnemies().size())
	{
		size = enemies.GetEnemies().size();
	}
	// ���b�N�I���\�� <= �G�̑��� �Ȃ�
	else if (tgt_datas_.size() <= enemies.GetEnemies().size())
	{
		size = tgt_datas_.size();
	}

	// �^�[�Q�b�g������܂ōŒZ�����̓G�����i�[
	for (UINT i = 0; i < size; i++)
	{
		tgt_datas_[i] = dist_sort[i];
	}
}

float LockOnSystem::CalcDistance(const XMFLOAT3 &dist_pos, const XMFLOAT3 &src_pos)
{
	XMVECTOR dist =
	{
		dist_pos.x - src_pos.x,
		dist_pos.y - src_pos.y,
		dist_pos.z - src_pos.z,
	};

	XMVECTOR len = XMVector3Length(dist);

	return len.m128_f32[0];
}
