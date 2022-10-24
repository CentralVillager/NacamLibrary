#include "Player.h"
#include "../../Utility/NcmUtil.h"
#include "../../Debug/NcmImGui.h"
#include "../../Math/Easing/NcmEasing.h"
#include "../../../Lib/Input/KeyboardInput.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace NcmUtill;
using namespace NcmMath;

std::unique_ptr<Model> Player::model_ = nullptr;
std::unique_ptr<Model> Player::coll_model_ = nullptr;

Player::Player()
	: AbsUniqueObj(1.0f, 2.0f),
	is_invincible_(false),
	taking_damage_trigger_(false),
	hp_(),
	mi_mgr_(nullptr),
	lockon_sys_(nullptr),
	charge_time_(40),
	count_(),
	ease_rot_right_(),
	ease_rot_left_(),
	ease_reset_rot_(),
	is_already_(false),
	is_released(false)
{}

Player::~Player()
{}

void Player::LoadResources()
{
	if (!model_)
	{
		model_ = std::make_unique<Model>();
		model_->LoadObjModel("Resources/low_fighter/", "new_low_fighter.obj", "new_low_fighter.mtl");

		coll_model_ = std::make_unique<Model>();
		coll_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void Player::Initialize()
{}

void Player::Initialize(MissileManager *mi_mgr, LockOnSystem *lockon_sys, XMFLOAT3 pos)
{
	is_dead_ = false;
	hp_ = 3;

	// 他クラス情報を格納
	mi_mgr_ = mi_mgr;
	lockon_sys_ = lockon_sys;

	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(pos);
	obj_->SetRot(XMFLOAT3(0, 180.0f, 0));

	// コリジョンの更新
	UpdateColl();

	// イージング関連の初期化
	EaseArgs args;
	args.init_value = 0.0f;
	args.total_move = 45.0f;
	args.ease_type = EaseType::OutCubic;
	ease_rot_right_ = NcmEasing::RegisterEaseData(args);

	args.init_value = 0.0f;
	args.total_move = -45.0f;
	args.ease_type = EaseType::OutCubic;
	ease_rot_left_ = NcmEasing::RegisterEaseData(args);

	args.init_value = 0.0f;
	args.total_move = -45.0f;
	args.ease_type = EaseType::OutCubic;
	ease_reset_rot_ = NcmEasing::RegisterEaseData(args);
}

void Player::Finalize()
{}

void Player::Update()
{
	if (is_dead_)
	{
		return;
	}

	if (KeyboardInput::HoldKey(DIK_SPACE))
	{
		ChargeMissile();
	}
	else if (KeyboardInput::ReleaseKey(DIK_SPACE))
	{
		FireMissile();
		lockon_sys_->ResetTargetNum();
		count_ = 0;
	}

	if (IsZeroOrLess(hp_))
	{
		is_dead_ = true;
	}

	if (taking_damage_trigger_)
	{
		is_invincible_ = true;
		taking_damage_trigger_ = false;
	}

	if (is_invincible_)
	{
		CountInvincibleTime();
	}

	obj_->Update();
	UpdateColl();
}

void Player::Draw()
{
	if (is_dead_)
	{
		return;
	}

	if (is_invincible_)
	{
		static uint32_t count = 0;
		count++;

		if (IsZero(count % 5))
		{
			obj_->Draw();
			count = 0;
		}
	}
	else
	{
		obj_->Draw();
	}
}

void Player::DrawColl()
{
	coll_obj_->Draw();
}

void Player::DebugDraw()
{
	ImGui::Text("HP : %d", hp_);
	ImGui::Text("count : %d", count_);
	ImGui::Checkbox("invi", &is_invincible_);
	ImGui::DragInt("charge_time", &charge_time_);
}

void Player::FireMissile()
{
	MissileArgs l_args{};
	l_args.pos = obj_->GetPos();
	l_args.vel = XMFLOAT3(0, 0, 1.0f);
	l_args.acc = XMFLOAT3(0, 0, 0);
	// tgt_pos はMissileManagerで設定
	// tgt_index はMissileManagerで設定
	l_args.detection_range = 1000.0f;
	l_args.init_straight_time_ = 0;
	l_args.life = 100;

	mi_mgr_->FireMultiMissile(l_args);
}

void Player::ChargeMissile()
{
	if (lockon_sys_->GetCurrentTgtNum() >= lockon_sys_->GetMaxTgtNum())
	{
		return;
	}

	count_++;

	if (count_ >= charge_time_)
	{
		lockon_sys_->AddTargetNum();
		count_ = 0;
	}
}

void Player::TakeDamage()
{
	if (is_invincible_)
	{
		return;
	}

	hp_--;
	taking_damage_trigger_ = true;
}

void Player::CountInvincibleTime()
{
	static uint32_t count = invincible_time_;
	count--;

	if (IsZeroOrLess(count))
	{
		is_invincible_ = false;
		count = invincible_time_;
	}
}

void Player::Move(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{
		XMFLOAT3 pos = obj_->GetPos();

		if (KeyboardInput::PushKey(DIK_W)) { pos.y += speed; }
		else if (KeyboardInput::PushKey(DIK_S)) { pos.y -= speed; }
		if (KeyboardInput::PushKey(DIK_D)) { pos.x += speed; }
		else if (KeyboardInput::PushKey(DIK_A)) { pos.x -= speed; }
		if (KeyboardInput::PushKey(DIK_R)) { pos.z += speed; }
		else if (KeyboardInput::PushKey(DIK_F)) { pos.z -= speed; }

		obj_->SetPos(pos);
	}

	if (KeyboardInput::PushKey(DIK_UP) || KeyboardInput::PushKey(DIK_DOWN) || KeyboardInput::PushKey(DIK_LEFT) || KeyboardInput::PushKey(DIK_RIGHT))
	{
		XMFLOAT3 rot = obj_->GetRot();

		if (KeyboardInput::PushKey(DIK_UP)) { rot.x -= speed; }
		if (KeyboardInput::PushKey(DIK_DOWN)) { rot.x += speed; }
		if (KeyboardInput::PushKey(DIK_LEFT)) { rot.z -= speed; }
		if (KeyboardInput::PushKey(DIK_RIGHT)) { rot.z += speed; }

		obj_->SetRot(rot);
	}
}

void Player::MoveXY(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A))
	{
		XMFLOAT3 pos = obj_->GetPos();

		if (KeyboardInput::PushKey(DIK_W)) { pos.y += speed; }
		else if (KeyboardInput::PushKey(DIK_S)) { pos.y -= speed; }
		if (KeyboardInput::PushKey(DIK_D)) { pos.x += speed; }
		else if (KeyboardInput::PushKey(DIK_A)) { pos.x -= speed; }

		obj_->SetPos(pos);
	}
}

void Player::MoveXZ(float speed)
{
	// 現在の位置を取得
	XMFLOAT3 pos = obj_->GetPos();

	// 前後移動
	if (KeyboardInput::PushKey(DIK_W)) { pos.z += speed; }
	else if (KeyboardInput::PushKey(DIK_S)) { pos.z -= speed; }

	// 左右移動
	if (KeyboardInput::PushKey(DIK_D))
	{
		pos.x += speed;
		RotPoseRight();
		is_released = false;
	}
	else if (KeyboardInput::PushKey(DIK_A))
	{
		pos.x -= speed;
		RotPoseLeft();
		is_released = false;
	}

	// 位置を更新
	obj_->SetPos(pos);

	if (KeyboardInput::ReleaseKey(DIK_A) || KeyboardInput::ReleaseKey(DIK_D))
	{
		is_released = true;
		NcmEasing::ResetTime(ease_rot_right_);
		NcmEasing::ResetTime(ease_rot_left_);
	}

	if (is_released)
	{
		ResetRotPose();
	}
	else
	{
		NcmEasing::ResetTime(ease_reset_rot_);
	}
}

void Player::RotPoseLeft()
{
	is_already_ = false;
	NcmEasing::UpdateValue(ease_rot_left_);
	obj_->SetRot(XMFLOAT3(0, 180.0f, NcmEasing::GetValue(ease_rot_left_)));
}

void Player::RotPoseRight()
{
	is_already_ = false;
	NcmEasing::UpdateValue(ease_rot_right_);
	obj_->SetRot(XMFLOAT3(0, 180.0f, NcmEasing::GetValue(ease_rot_right_)));
}

void Player::ResetRotPose()
{
	if (!is_already_)
	{
		float rot = obj_->GetRot().z;
		NcmEasing::SetInitValue(ease_reset_rot_, rot);

		// それが正なら
		if (IsPlus(rot))
		{
			NcmEasing::SetTotalMove(ease_reset_rot_, -45.0f);
		}
		// それが負なら
		else if (IsMinus(rot))
		{
			NcmEasing::SetTotalMove(ease_reset_rot_, 45.0f);
		}

		is_already_ = true;
	}

	NcmEasing::UpdateValue(ease_reset_rot_);
	obj_->SetRot(XMFLOAT3(0, 180.0f, NcmEasing::GetValue(ease_reset_rot_)));
}
