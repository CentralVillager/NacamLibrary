#include "LockOnSystem.h"
#include "../Player/Player.h"
#include "../../Debug/NcmImGui.h"
#include "../../../Lib/PreDraw/PreDraw.h"

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
	// 最も近い敵を算出する
	CalcNearestTargets(player_ptr_->GetPos(), *enemies_ptr_);

	// ターゲットリストのイテレータを取得
	auto itr = near_tgt_list_.begin();

	// 全てのマーカーに対して
	for (auto &i : markers_)
	{
		// ターゲットの位置にマーカーを設置
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

	numbers_->DrawNumber(current_tgt_num_, 1.0f, Right, Bottom);
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
	// 現ターゲット数が最大ターゲット数以上なら
	if (current_tgt_num_ >= max_tgt_num_)
	{
		// その後の処理をスルー
		return;
	}

	// マーカーを初期化
	markers_.emplace_back();
	markers_.back().Initialize();
	markers_.back().SetModel(model_.get());
	markers_.back().SetScale(2.0f);

	// 現ターゲット数を更新
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
	// 初期化
	near_tgt_list_.clear();

	// 全ての敵に対して
	for (UINT i = 0; i < enemies.GetEnemies().size(); i++)
	{
		// 要素を構築
		near_tgt_list_.emplace_back();
		// プレイヤーと敵との距離を計算し、格納
		near_tgt_list_.back().dist = CalcDistance(player_pos, enemies.GetPos(i));
		// 位置を格納
		near_tgt_list_.back().pos = enemies.GetPos(i);
		// IDを格納
		near_tgt_list_.back().id = enemies.GetID(i);
	}

	// 距離が近い順に要素をソート
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
