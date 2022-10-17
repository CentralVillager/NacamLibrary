#include "Player.h"
#include "../../Utility/NcmUtil.h"
#include "../../Debug/NcmImGui.h"
#include "../../Math/Easing/NcmEasing.h"
#include "../../../Lib/Input/KeyboardInput.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace NcmUtill;

std::shared_ptr<Model> Player::model_ = nullptr;
std::shared_ptr<Model> Player::sphere_model_ = nullptr;

Player::Player()
{
	object_ = std::make_shared<Object3d>();
	sphere_obj_ = std::make_shared<Object3d>();
}

Player::~Player()
{}

void Player::LoadResources()
{
	if (!model_)
	{
		model_ = std::make_shared<Model>();
		model_->LoadObjModel("Resources/low_fighter/", "new_low_fighter.obj", "new_low_fighter.mtl");

		sphere_model_ = std::make_shared<Model>();
		sphere_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void Player::Initialize(MissileManager *mi_mgr, LockOnSystem *lockon_sys)
{
	// 他クラス情報を格納
	mi_mgr_ = mi_mgr;
	lockon_sys_ = lockon_sys;

	// 描画データの初期化
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetPosition(XMFLOAT3(0, 0, 0));
	object_->SetRotation(XMFLOAT3(0, 180.0f, 0));
	object_->SetScale(1.0f);
	sphere_obj_->Initialize();
	sphere_obj_->SetModel(sphere_model_.get());

	// コリジョンの更新
	UpdateCollision();

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
	if (KeyboardInput::HoldKey(DIK_SPACE))
	{
		ChargeMissile();
	}
	else if (KeyboardInput::ReleaseKey(DIK_SPACE))
	{
		FireMissile();
		lockon_sys_->ResetTargetNum();
		count = 0;
	}

	object_->Update();
	UpdateCollision();
}

void Player::Draw()
{
	object_->Draw();
}

void Player::DrawColl()
{
	sphere_obj_->Draw();
}

void Player::DebugDraw()
{
	ImGui::Text("count : %d", count);
	ImGui::DragInt("charge_time", &charge_time);
	ImGui::Text("ease_value : %f", NcmEasing::GetValue(0));
}

void Player::FireMissile()
{
	MissileArgs l_args{};
	l_args.pos = object_->GetPosition();
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
	count++;

	if (count >= charge_time)
	{
		lockon_sys_->AddTargetNum();
		count = 0;
	}
}

void Player::Move(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{
		XMFLOAT3 pos = object_->GetPosition();

		if (KeyboardInput::PushKey(DIK_W)) { pos.y += speed; }
		else if (KeyboardInput::PushKey(DIK_S)) { pos.y -= speed; }
		if (KeyboardInput::PushKey(DIK_D)) { pos.x += speed; }
		else if (KeyboardInput::PushKey(DIK_A)) { pos.x -= speed; }
		if (KeyboardInput::PushKey(DIK_R)) { pos.z += speed; }
		else if (KeyboardInput::PushKey(DIK_F)) { pos.z -= speed; }

		object_->SetPosition(pos);
	}

	if (KeyboardInput::PushKey(DIK_UP) || KeyboardInput::PushKey(DIK_DOWN) || KeyboardInput::PushKey(DIK_LEFT) || KeyboardInput::PushKey(DIK_RIGHT))
	{
		XMFLOAT3 rot = object_->GetRotation();

		if (KeyboardInput::PushKey(DIK_UP)) { rot.x -= speed; }
		if (KeyboardInput::PushKey(DIK_DOWN)) { rot.x += speed; }
		if (KeyboardInput::PushKey(DIK_LEFT)) { rot.z -= speed; }
		if (KeyboardInput::PushKey(DIK_RIGHT)) { rot.z += speed; }

		object_->SetRotation(rot);
	}
}

void Player::MoveXY(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A))
	{
		XMFLOAT3 pos = object_->GetPosition();

		if (KeyboardInput::PushKey(DIK_W)) { pos.y += speed; }
		else if (KeyboardInput::PushKey(DIK_S)) { pos.y -= speed; }
		if (KeyboardInput::PushKey(DIK_D)) { pos.x += speed; }
		else if (KeyboardInput::PushKey(DIK_A)) { pos.x -= speed; }

		object_->SetPosition(pos);
	}
}

void Player::MoveXZ(float speed)
{
	// WASDの入力を検知したら
	//if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A))
	{
		// 現在の位置を取得
		XMFLOAT3 pos = object_->GetPosition();

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
		object_->SetPosition(pos);
	}
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

void Player::UpdateCollision()
{
	coll_.center = XMLoadFloat3(&object_->GetPosition());
	coll_.radius = COLL_RADIUS_;

	sphere_obj_->SetPosition(ToFloat3(coll_.center));
	sphere_obj_->SetScale(coll_.radius);
	sphere_obj_->Update();
}

void Player::RotPoseLeft()
{
	is_already_ = false;
	NcmEasing::UpdateValue(ease_rot_left_);
	object_->SetRotation(XMFLOAT3(0, 180.0f, NcmEasing::GetValue(ease_rot_left_)));
}

void Player::RotPoseRight()
{
	is_already_ = false;
	NcmEasing::UpdateValue(ease_rot_right_);
	object_->SetRotation(XMFLOAT3(0, 180.0f, NcmEasing::GetValue(ease_rot_right_)));
}

void Player::ResetRotPose()
{
	if (!is_already_)
	{
		float rot = object_->GetRotation().z;
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
	object_->SetRotation(XMFLOAT3(0, 180.0f, NcmEasing::GetValue(ease_reset_rot_)));
}
