#include "Bullet.h"
#include "../Sources/App/Utility/NcmUtil.h"
#include "../../Debug/NcmImGui.h"

using namespace DirectX;
using namespace NcmUtill;

std::unique_ptr<Model> Bullet::model_ = nullptr;
std::unique_ptr<Model> Bullet::sphere_model_ = nullptr;

Bullet::Bullet()
{
	object_ = std::make_unique<Object3d>();
	sphere_obj_ = std::make_unique<Object3d>();
}

Bullet::~Bullet()
{}

void Bullet::LoadResources()
{
	// 他のクラスでも同じ処理をしている

	if (!model_)
	{
		model_ = std::make_unique<Model>();
		model_->LoadObjModel("Resources/bullet/", "bullet.obj", "bullet.mtl");
	}

	if (!sphere_model_)
	{
		sphere_model_ = std::make_unique<Model>();
		sphere_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void Bullet::Initialize(const XMFLOAT3 &pos)
{
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetPosition(pos);

	sphere_obj_->Initialize();
	sphere_obj_->SetModel(sphere_model_.get());
}

void Bullet::Update()
{
	if (is_dead_) { return; }

	life_--;

	pos_ = object_->GetPosition();

	pos_.x += vel_.x;
	pos_.y += vel_.y;
	pos_.z += vel_.z;

	object_->SetPosition(pos_);
	object_->Update();
	UpdateCollision();

	if (IsZeroOrLess(life_))
	{
		//is_dead_ = true;
	}
}

void Bullet::Draw()
{
	if (is_dead_) { return; }
	object_->Draw();
}

void Bullet::DrawColl()
{
	if (is_dead_) { return; }
	sphere_obj_->Draw();
}

void Bullet::DebugDraw()
{
	ImGui::Text("pos : (%f, %f, %f)", object_->GetPosition().x, object_->GetPosition().y, object_->GetPosition().z);
}

void Bullet::Fire(const XMFLOAT3 &dist)
{
	is_dead_ = false;

	CalcAngle(dist);
}

void Bullet::CalcAngle(const XMFLOAT3 &dist)
{
	XMFLOAT3 pos = object_->GetPosition();

	// XMVECTORに変換
	XMVECTOR bl_vec = XMLoadFloat3(&object_->GetPosition());
	XMVECTOR di_vec = XMLoadFloat3(&dist);

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR vec =
	{
		(di_vec.m128_f32[0] - bl_vec.m128_f32[0]),
		(di_vec.m128_f32[1] - bl_vec.m128_f32[1]),
		(di_vec.m128_f32[2] - bl_vec.m128_f32[2])
	};

	// 正規化
	XMVECTOR norm_vec = XMVector3Normalize(vec);

	XMStoreFloat3(&vel_, norm_vec);

	vel_.x *= SPEED_;
	vel_.y *= SPEED_;
	vel_.z *= SPEED_;
}

void Bullet::UpdateCollision()
{
	coll_.center = XMLoadFloat3(&object_->GetPosition());
	coll_.radius = COLL_RADIUS_;

	sphere_obj_->SetPosition(ToFloat3(coll_.center));
	sphere_obj_->SetScale(coll_.radius);
	sphere_obj_->Update();
}
