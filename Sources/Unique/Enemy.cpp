#include "Enemy.h"
#include "../Utility/Utility.h"

using namespace NcmUtill;

std::unique_ptr<Model> Enemy::model_ = nullptr;
std::unique_ptr<Model> Enemy::sphere_model_ = nullptr;

Enemy::Enemy()
{
	object_ = std::make_unique<Object3d>();
	sphere_obj_ = std::make_shared<Object3d>();
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
}

void Enemy::Initialize(XMFLOAT3 pos)
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
}

void Enemy::Finalize()
{}

void Enemy::Update()
{
	RotY();
	MoveHorizontally(0.5f, 100.0f);
	object_->Update();
	UpdateCollision();
}

void Enemy::Draw()
{
	object_->Draw();
}

void Enemy::DrawColl()
{
	sphere_obj_->Draw();
}

void Enemy::DebugDraw()
{}

void Enemy::RotY()
{
	XMFLOAT3 rot = object_->GetRotation();
	rot.y += 1.0f;
	object_->SetRotation(rot);
}

void Enemy::MoveHorizontally(float speed, float range)
{
	count_--;

	XMFLOAT3 pos = object_->GetPosition();
	pos.x += speed_;
	object_->SetPosition(pos);

	if (IsZero(count_))
	{
		// •„†‚ð”½“]
		speed_ *= -1.0f;
		count_ = 100 * 2;
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
