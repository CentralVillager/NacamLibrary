#include "LockOnSystem.h"
#include "Player.h"

using namespace DirectX;

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

void LockOnSystem::Initialize(Player *player, EnemiesList *enemies_ptr, int multi_tgt_n)
{
	player_ptr_ = player;
	enemies_ptr_ = enemies_ptr;

	multi_target_num_ = multi_tgt_n;
	for (UINT i = 0; i < multi_target_num_; i++)
	{
		markers_.emplace_back();
		markers_.back().Initialize();
		markers_.back().SetModel(model_.get());
		markers_.back().SetScale(2.0f);

		tgt_datas_.emplace_back();
	}
	tgt_index_ = 0;
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

void LockOnSystem::CalcNearestTargets(const XMFLOAT3 &player_pos, EnemiesList &enemies)
{
	// 仮初期化
	float nearest_dist = 1000000.0f;

	// 敵がいないなら
	if (enemies.GetSize() <= 0)
	{
		// 正面奥にターゲットを置く
		tgt_datas_[0].pos = XMFLOAT3(0, 0, -100.0f);
	}

	std::vector<TargetData> dist_sort;

	// 全ての敵に対して
	for (UINT i = 0; i < enemies.GetSize(); i++)
	{
		// 距離を計算、各データを格納
		dist_sort.emplace_back();
		dist_sort.back().dist = CalcDistance(player_pos, enemies.GetPos(i));
		dist_sort.back().pos = enemies.GetPos(i);
		dist_sort.back().index = i;
	}

	// distに対して昇順ソート
	std::sort(dist_sort.begin(), dist_sort.end(),
		[](const TargetData &a, const TargetData &b) { return a.dist < b.dist; });

	// ターゲット上限数まで最短距離の敵情報を格納
	for (UINT i = 0; i < tgt_datas_.size(); i++)
	{
		tgt_datas_[i] = dist_sort[i];
	}
}

float LockOnSystem::CalcDistance(XMFLOAT3 dist_pos, XMFLOAT3 src_pos)
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
