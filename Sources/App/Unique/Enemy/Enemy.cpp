#include "Enemy.h"
#include <cmath>
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Player/Player.h"
#include "../../../Lib/DirectX/PreDraw.h"
#include "../Sources/App/Math/NcmMath.h"
#include "../Missile/MissileLauncher.h"
#include "../../DebugUtill/NcmDebug.h"

using namespace NcmUtill;
using namespace NcmMath;

std::unique_ptr<Model> Enemy::model_ = nullptr;
std::unique_ptr<Model> Enemy::coll_model_ = nullptr;
int Enemy::id_counter_ = -1;
Player *Enemy::player_ = nullptr;

Enemy::Enemy() :
	AbsUniqueObj(3.0f, 2.0f),
	id_(0),
	count_(100),
	circular_angle_(0.0f),
	mi_launcher_(std::make_shared<MissileLauncher>()),
	shot_interval_(0),
	missile_launch_count_(LAUNCH_MISSILE_INTERVAL_)
{
	// 敵生成時にカウントを加算
	id_counter_++;
	// それをIDとする
	id_ = id_counter_;
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

	circular_angle_ = 0.0f;

	speed_ = player_->GetSpeed();
}

void Enemy::Initialize()
{}

void Enemy::Finalize()
{}

void Enemy::Update()
{
	// Y軸回転
	RotY();

	// 水平反復移動
	MoveHorizontally(2.0f, 100.0f);

	// チートモードでなければ
	if (!NcmDebug::GetInstance()->IsCheatMode())
	{
		// ミサイルを一定間隔で発射する
		LaunchMissileSequence();
	}

	// Z方向へプレイヤーと同じ速度で移動
	MoveZ(player_->GetSpeed());

	obj_->Update();
	UpdateColl();
}

void Enemy::Draw()
{
	PreDraw::SetPipeline(PipelineName::Object3d_WireFrame);
	obj_->Draw();
}

void Enemy::DrawColl()
{
	coll_obj_->Draw();
}

void Enemy::DebugDraw()
{
	ImGui::Text("ID : %d", id_);
	ImGui::Text("interval : %d", shot_interval_);
	ImGui::DragInt("cycle", &cycle, 1, 1, 10000);
	ImGui::DragFloat("length", &length, 1.0f, 0.0f, 100.0f);
	ImGui::Separator();
}

void Enemy::RotY()
{
	XMFLOAT3 rot = obj_->GetRot();
	rot.y += 1.0f;
	obj_->SetRot(rot);
}

void Enemy::MoveHorizontally(const float speed, const float range)
{
	count_--;

	XMFLOAT3 pos = obj_->GetPos();
	static int t = 0;
	t++;
	float sin = GetSinWave(2000, (float)(t));
	pos.x += sin;
	obj_->SetPos(pos);

	if (IsZero(count_))
	{
		// 符号を反転
		speed_ *= -1.0f;
		count_ = 100 * 2;
	}
}

void Enemy::MoveZ(const float speed)
{
	XMFLOAT3 pos = obj_->GetPos();
	pos.z += speed;
	obj_->SetPos(pos);
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

void Enemy::FireMissile()
{
	mi_launcher_->FireMissile(MissileType::ForEnemy, LaunchedBy::Enemy, GetPos());
}

void Enemy::LaunchMissileSequence()
{
	if (CheckCanFire())
	{
		mi_launcher_->FireMissile(MissileType::ForEnemy, LaunchedBy::Enemy, GetPos());
	}
}

bool Enemy::CheckCanFire()
{
	missile_launch_count_--;

	if (IsZeroOrLess(missile_launch_count_))
	{
		missile_launch_count_ = LAUNCH_MISSILE_INTERVAL_;
		return true;
	}

	return false;
}
