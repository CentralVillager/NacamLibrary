#include "LockOnSystem.h"
#include "../Player/Player.h"
#include "../../Lib/DebugUtill/NcmImGui.h"
#include "../../Lib/DirectX/PreDraw.h"

using namespace DirectX;

uint32_t LockOnSystem::current_tgt_num_ = 0;
uint32_t LockOnSystem::max_tgt_num_ = 0;
std::unique_ptr<Model> LockOnSystem::model_ = nullptr;

LockOnSystem::LockOnSystem() :
	markers_(),
	near_tgt_list_(),
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
	max_tgt_num_ = 4;
}

void LockOnSystem::Update()
{
	// �ł��߂��G���Z�o����
	CalcNearestTargets(player_ptr_->GetPos(), *enemies_ptr_);

	// �^�[�Q�b�g���X�g�̃C�e���[�^���擾
	auto itr = near_tgt_list_.begin();

	// �S�Ẵ}�[�J�[�ɑ΂���
	for (auto &i : markers_)
	{
		// �^�[�Q�b�g�̈ʒu�Ƀ}�[�J�[��ݒu
		i.SetPos({ itr->pos.x, itr->pos.y + 2.0f, itr->pos.z });
		i.Update();
		itr++;
	}
}

void LockOnSystem::Draw()
{
	PreDraw::SetPipeline(PipelineName::Object3d);
	for (auto &i : markers_)
	{
		i.Draw();
	}
}

void LockOnSystem::DrawNum()
{
	using enum HorizontalAlignment;
	using enum VerticalAlignment;

	numbers_->DrawNumber(current_tgt_num_, 1.0f, 0.5f, Right, Bottom);
}

void LockOnSystem::DebugDraw()
{
	ImGui::Text("target_num : %d", current_tgt_num_);
}

const uint32_t LockOnSystem::GetMaxTgtNum()
{
	if (enemies_ptr_->GetEnemies().size() < max_tgt_num_)
	{
		return (uint32_t)(enemies_ptr_->GetEnemies().size());
	}
	else
	{
		return max_tgt_num_;
	}
}

void LockOnSystem::AddTargetNum()
{
	// ���^�[�Q�b�g�����ő�^�[�Q�b�g���ȏ�Ȃ�
	if (current_tgt_num_ >= max_tgt_num_)
	{
		// ���̌�̏������X���[
		return;
	}

	// �}�[�J�[��������
	markers_.emplace_back();
	markers_.back().Initialize();
	markers_.back().SetModel(model_.get());
	markers_.back().SetScale(2.0f);

	// ���^�[�Q�b�g�����X�V
	current_tgt_num_++;
}

void LockOnSystem::ResetTargetNum()
{
	markers_.clear();
	near_tgt_list_.clear();
	current_tgt_num_ = 0;
}

void LockOnSystem::CalcNearestTargets(const XMFLOAT3 &player_pos, EnemiesList &enemies)
{
	// ������
	near_tgt_list_.clear();

	// �S�Ă̓G�ɑ΂���
	for (UINT i = 0; i < enemies.GetEnemies().size(); i++)
	{
		// �v�f���\�z
		near_tgt_list_.emplace_back();
		// �v���C���[�ƓG�Ƃ̋������v�Z���A�i�[
		near_tgt_list_.back().dist = CalcDistance(player_pos, enemies.GetPos(i));
		// �ʒu���i�[
		near_tgt_list_.back().pos = enemies.GetPos(i);
		// ID���i�[
		near_tgt_list_.back().id = enemies.GetID(i);
	}

	// �������߂����ɗv�f���\�[�g
	near_tgt_list_.sort([](const TargetData &lhs, const TargetData &rhs) { return lhs.dist < rhs.dist; });
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