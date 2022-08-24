#include "LockOnSystem.h"
#include "Player.h"

using namespace DirectX;

std::unique_ptr<Model> LockOnSystem::model_ = nullptr;

LockOnSystem::LockOnSystem()
{
	marker_ = std::make_unique<Object3d>();
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

void LockOnSystem::Initialize(Player *player, EnemiesList *enemies_ptr)
{
	player_ptr_ = player;
	enemies_ptr_ = enemies_ptr;

	marker_->Initialize();
	marker_->SetModel(model_.get());
	marker_->SetScale(2.0f);

	tgt_index_ = 0;
}

void LockOnSystem::Update()
{
	CalcNearestTarget(player_ptr_->GetPos(), *enemies_ptr_);
	marker_->SetPosition({ target_pos_.x, target_pos_.y + 2.0f, target_pos_.z });
	marker_->Update();
}

void LockOnSystem::Draw()
{
	marker_->Draw();
}

void LockOnSystem::CalcNearestTarget(const XMFLOAT3 &player_pos, EnemiesList &enemies)
{
	static float old_dist = 1000000.0f;

	for (UINT i = 0; i < enemies.GetSize(); i++)
	{
		float dist = CalcDistance(player_pos, enemies.GetPos(i));

		if (old_dist > dist)
		{
			target_pos_ = enemies.GetPos(i);
			tgt_index_ = i;
		}

		old_dist = dist;
	}
}

float LockOnSystem::CalcDistance(XMFLOAT3 dist_pos, XMFLOAT3 src_pos)
{
	XMVECTOR dist = {

		dist_pos.x - src_pos.x,
		dist_pos.y - src_pos.y,
		dist_pos.z - src_pos.z,
	};

	XMVECTOR len = XMVector3Length(dist);

	return len.m128_f32[0];
}
