#include "Bullet.h"
#include "../Sources/App/Utility/NcmUtil.h"

using namespace DirectX;
using namespace NcmUtill;

Bullet::Bullet() : AbsUniqueObj({0, 0, 0}, 3.0f)
{
	obj_ = std::make_unique<Object3d>();
	coll_obj_ = std::make_unique<Object3d>();
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

	if (!coll_model_)
	{
		coll_model_ = std::make_unique<Model>();
		coll_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void Bullet::Initialize()
{
	obj_->Initialize();
	obj_->SetModel(model_.get());

	coll_obj_->Initialize();
	coll_obj_->SetModel(coll_model_.get());
}

void Bullet::Update()
{
	if (is_dead_) { return; }

	life_--;

	pos_ = obj_->GetPosition();

	pos_.x += vel_.x;
	pos_.y += vel_.y;
	pos_.z += vel_.z;

	obj_->SetPosition(pos_);
	obj_->Update();
	UpdateColl();

	if (IsZeroOrLess(life_))
	{
		//is_dead_ = true;
	}
}

void Bullet::Draw()
{
	if (is_dead_) { return; }
	obj_->Draw();
}

void Bullet::DrawColl()
{
	if (is_dead_) { return; }
	obj_->Draw();
}

void Bullet::DebugDraw()
{
	ImGui::Text("pos : (%f, %f, %f)", obj_->GetPosition().x, obj_->GetPosition().y, obj_->GetPosition().z);
}

void Bullet::Fire(const XMFLOAT3 &dist)
{
	is_dead_ = false;

	CalcAngle(dist);
}

void Bullet::CalcAngle(const XMFLOAT3 &dist)
{
	XMFLOAT3 pos = obj_->GetPosition();

	// XMVECTORに変換
	XMVECTOR bl_vec = XMLoadFloat3(&obj_->GetPosition());
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

	vel_.x *= speed_;
	vel_.y *= speed_;
	vel_.z *= speed_;
}
