#include "Enemy.h"
#include <cmath>
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Player/Player.h"

using namespace NcmUtill;

int Enemy::id_counter_ = -1;
Player *Enemy::player_ = nullptr;

Enemy::Enemy()
	: AbsUniqueObj({ 0, 0, 0 }, 0.5f),
	id_(0),
	count_(100),
	circular_angle_(0.0f),
	bullet_(std::make_shared<Bullet>()),
	shot_interval_(0)
{
	id_counter_++;
}

Enemy::~Enemy()
{}

void Enemy::LoadResources()
{
	if (!model_)
	{
		model_ = std::make_unique<Model>();
		model_->LoadObjModel("Resources/enemy/", "enemy.obj", "enemy.mtl");

		coll_model_ = std::make_unique<Model>();
		coll_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}

	Bullet::LoadResources();
}

void Enemy::ImportPtr(Player *player)
{
	player_ = player;
}

void Enemy::Initialize(const XMFLOAT3 &pos)
{
	InitializeObj();
	obj_->SetPosition(pos);

	is_dead_ = false;
	id_ = id_counter_;

	circular_angle_ = 0.0f;

	bullet_->Initialize();
	bullet_->SetPos(pos);
}

void Enemy::Initialize()
{}

void Enemy::Finalize()
{}

void Enemy::Update()
{
	RotY();
	MoveHorizontally(0.5f, 100.0f);
	AutoShot(30, player_->GetPos());
	obj_->Update();
	UpdateColl();
	
	bullet_->Update();
}

void Enemy::Draw()
{
	obj_->Draw();
	bullet_->Draw();
}

void Enemy::DrawColl()
{
	coll_obj_->Draw();
	bullet_->DrawColl();
}

void Enemy::DebugDraw()
{
	bullet_->DebugDraw();
}

void Enemy::RotY()
{
	XMFLOAT3 rot = obj_->GetRotation();
	rot.y += 1.0f;
	obj_->SetRotation(rot);
}

void Enemy::MoveHorizontally(const float &speed, const float &range)
{
	count_--;

	XMFLOAT3 pos = obj_->GetPosition();
	pos.x += speed_;
	obj_->SetPosition(pos);

	if (IsZero(count_))
	{
		// 符号を反転
		speed_ *= -1.0f;
		count_ = 100 * 2;
	}
}

void Enemy::MoveCircular()
{
	float origin = 0.0f;
	float radius = 10.0f;

	XMFLOAT3 speed =
	{
		radius * cosf(circular_angle_),
		0,
		radius * sinf(circular_angle_)
	};

	XMFLOAT3 pos = obj_->GetPosition();
	pos.x += speed.x;
	pos.z += speed.z;
	obj_->SetPosition(pos);

	static int count = 10;
	count--;

	if (count <= 0)
	{
		circular_angle_ += 0.1f;
		count = 10;
	}
}

void Enemy::AutoShot(int interval, const XMFLOAT3 &pos)
{
	// 間隔が変更されていたら
	if (shot_interval_ != interval)
	{
		// 間隔を更新する
		shot_interval_ = interval;
	}

	static int count = shot_interval_;

	count--;

	// カウントが0以下になったら
	if (IsZeroOrLess(count))
	{
		// 弾を発射する
		bullet_->Fire(pos);
		// カウントをリセットする
		count = shot_interval_;
	}
}
