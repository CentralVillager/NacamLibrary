#include "Enemy.h"
#include <cmath>
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Player/Player.h"
#include "../../../Lib/PreDraw/PreDraw.h"

using namespace NcmUtill;
using namespace NcmMath;

std::unique_ptr<Model> Enemy::model_ = nullptr;
std::unique_ptr<Model> Enemy::coll_model_ = nullptr;
int Enemy::id_counter_ = -1;
Player *Enemy::player_ = nullptr;

Enemy::Enemy() :
	AbsUniqueObj(0.5f, 2.0f),
	id_(0),
	count_(100),
	circular_angle_(0.0f),
	bullets_(std::make_shared<BulletList>()),
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
	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(pos);

	is_dead_ = false;
	id_ = id_counter_;

	circular_angle_ = 0.0f;
}

void Enemy::Initialize()
{}

void Enemy::Finalize()
{}

void Enemy::Update()
{
	RotY();
	MoveHorizontally(0.5f, 100.0f);
	AutoShot(200, player_->GetPos());
	obj_->Update();
	UpdateColl();

	bullets_->Update();
}

void Enemy::Draw()
{
	PreDraw::PreRender(PipelineName::Object3d_WireFrame);
	obj_->Draw();

	PreDraw::PreRender(PipelineName::Object3d);
	bullets_->Draw();

	PreDraw::PreRender(PipelineName::Object3d_WireFrame);
}

void Enemy::DrawColl()
{
	coll_obj_->Draw();
	bullets_->DrawColl();
}

void Enemy::DebugDraw()
{
	ImGui::Text("ID : %d", id_);
	ImGui::Text("interval : %d", shot_interval_);
	ImGui::DragInt("cycle", &cycle, 1, 1, 10000);
	ImGui::DragFloat("length", &length, 1.0f, 0.0f, 100.0f);
	ImGui::Separator();
	bullets_->DebugDraw();
}

void Enemy::RotY()
{
	XMFLOAT3 rot = obj_->GetRot();
	rot.y += 1.0f;
	obj_->SetRot(rot);
}

void Enemy::MoveHorizontally(const float &speed, const float &range)
{
	count_--;

	XMFLOAT3 pos = obj_->GetPos();
	static int t = 0;
	t++;
	float sin = GetSinWave(2000, t);
	pos.x += sin;
	obj_->SetPos(pos);

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

	XMFLOAT3 pos = obj_->GetPos();
	pos.x += speed.x;
	pos.z += speed.z;
	obj_->SetPos(pos);

	static int count = 10;
	count--;

	if (count <= 0)
	{
		circular_angle_ += 0.1f;
		count = 10;
	}
}

void Enemy::AutoShot(int interval, const XMFLOAT3 &dist)
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
		bullets_->Fire(obj_->GetPos(), dist);
		// カウントをリセットする
		count = shot_interval_;
	}
}
