#include "Enemy.h"
#include <cmath>
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Player/Player.h"

using namespace NcmUtill;

std::unique_ptr<Model> Enemy::model_ = nullptr;
std::unique_ptr<Model> Enemy::sphere_model_ = nullptr;
int Enemy::id_counter_ = -1;
Player *Enemy::player_ = nullptr;

Enemy::Enemy()
{
	object_ = std::make_unique<Object3d>();
	sphere_obj_ = std::make_shared<Object3d>();
	bullet_ = std::make_shared<Bullet>();
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

		sphere_model_ = std::make_unique<Model>();
		sphere_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}

	Bullet::LoadResources();
}

void Enemy::ImportPtr(Player *player)
{
	player_ = player;
}

void Enemy::Initialize(const XMFLOAT3 &pos)
{
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetPosition(pos);
	object_->SetRotation(XMFLOAT3(0, 0, 0));
	object_->SetScale(1.0f);

	sphere_obj_->Initialize();
	sphere_obj_->SetModel(sphere_model_.get());

	UpdateCollision();

	is_dead_ = false;
	id_ = id_counter_;

	circular_angle_ = 0.0f;

	bullet_->Initialize(object_->GetPosition());
}

void Enemy::Finalize()
{}

void Enemy::Update()
{
	RotY();
	MoveHorizontally(0.5f, 100.0f);
	AutoShot(30, player_->GetPos());
	//MoveCircular();
	object_->Update();
	UpdateCollision();

	bullet_->Update();
}

void Enemy::Draw()
{
	object_->Draw();
	bullet_->Draw();
}

void Enemy::DrawColl()
{
	sphere_obj_->Draw();
	bullet_->DrawColl();
}

void Enemy::DebugDraw()
{
	bullet_->DebugDraw();
}

void Enemy::RotY()
{
	XMFLOAT3 rot = object_->GetRotation();
	rot.y += 1.0f;
	object_->SetRotation(rot);
}

void Enemy::MoveHorizontally(const float &speed, const float &range)
{
	count_--;

	XMFLOAT3 pos = object_->GetPosition();
	pos.x += speed_;
	object_->SetPosition(pos);

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

	XMFLOAT3 pos = object_->GetPosition();
	pos.x += speed.x;
	pos.z += speed.z;
	object_->SetPosition(pos);

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

void Enemy::UpdateCollision()
{
	coll_.center = XMLoadFloat3(&object_->GetPosition());
	coll_.radius = COLL_RADIUS_;

	sphere_obj_->SetPosition(ToFloat3(coll_.center));
	sphere_obj_->SetScale(coll_.radius);
	sphere_obj_->Update();
}
