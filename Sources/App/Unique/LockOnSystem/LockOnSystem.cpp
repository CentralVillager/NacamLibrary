#include "LockOnSystem.h"
#include "../Player/Player.h"
#include "../../Debug/NcmImGui.h"

using namespace DirectX;

uint32_t LockOnSystem::current_tgt_num_ = 0;
std::unique_ptr<Model> LockOnSystem::model_ = nullptr;

LockOnSystem::LockOnSystem() :
	markers_(),
	tgt_datas_(),
	player_ptr_(),
	enemies_ptr_(),
	numbers_(std::make_unique<Numbers>())
{}

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

void LockOnSystem::Initialize(Player *player, EnemiesList *enemies_ptr)
{
	player_ptr_ = player;
	enemies_ptr_ = enemies_ptr;

	numbers_->Initialize();
}

void LockOnSystem::Update()
{
	CalcNearestTargets(player_ptr_->GetPos(), *enemies_ptr_);

	auto itr = tgt_datas_.begin();
	for (auto &i : markers_)
	{
		i.SetPos({ itr->pos.x, itr->pos.y + 2.0f, itr->pos.z });
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

void LockOnSystem::DrawNum()
{
	using enum HorizontalAlignment;
	using enum VerticalAlignment;

	numbers_->DrawNumber(current_tgt_num_, 1.0f, Right, Bottom);
}

void LockOnSystem::DebugDraw()
{
	ImGui::Text("target_num : %d", current_tgt_num_);
}

void LockOnSystem::AddTargetNum()
{
	if (current_tgt_num_ >= max_tgt_num_)
	{
		return;
	}

	markers_.emplace_back();
	markers_.back().Initialize();
	markers_.back().SetModel(model_.get());
	markers_.back().SetScale(2.0f);

	tgt_datas_.emplace_back();
	current_tgt_num_++;
}

void LockOnSystem::ResetTargetNum()
{
	markers_.clear();
	tgt_datas_.clear();
	current_tgt_num_ = 0;
}

void LockOnSystem::CalcNearestTargets(const XMFLOAT3 &player_pos, EnemiesList &enemies)
{
	// ��������
	float nearest_dist = 1000000.0f;

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
