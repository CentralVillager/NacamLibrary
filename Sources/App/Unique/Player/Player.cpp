#include "Player.h"
#include "../../Utility/NcmUtil.h"
#include "../../Debug/NcmImGui.h"
#include "../../Math/Easing/NcmEasing.h"
#include "../../Debug/NcmDebug.h"
#include "../../../Lib/Input/KeyboardInput.h"
#include "../Sources/App/Math/NcmMath.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace NcmUtill;
using namespace NcmMath;

std::unique_ptr<Model> Player::model_ = nullptr;
std::unique_ptr<Model> Player::coll_model_ = nullptr;

Player::Player()
	: AbsUniqueObj(0.5f, 2.0f),
	is_invincible_(false),
	taking_damage_trigger_(false),
	is_triggering_ult_(false),
	hp_(),
	p_mi_mgr_(nullptr),
	p_lockon_sys_(nullptr),
	p_ult_(nullptr),
	charge_time_(40),
	count_(),
	ease_rot_right_(),
	ease_rot_left_(),
	ease_reset_rot_(),
	is_already_(false),
	is_released(false),
	rot_angle_(5.0f)
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
{
	is_dead_ = false;
	hp_ = 3;
	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(XMFLOAT3(0, 0, 20.0f));
	obj_->SetRot(XMFLOAT3(0, 180.0f, 0));

	// コリジョンの更新
	UpdateColl();

	// イージング関連の初期化
	NcmEaseDesc args;
	args.init_value = 0.0f;
	args.total_move = rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = 0.05f;
	ease_rot_right_ = NcmEasing::RegisterEaseData(args);

	args.init_value = 0.0f;
	args.total_move = -rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = 0.05f;
	ease_rot_left_ = NcmEasing::RegisterEaseData(args);

	args.init_value = 0.0f;
	args.total_move = -rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = 0.05f;
	ease_reset_rot_ = NcmEasing::RegisterEaseData(args);
}

void Player::Initialize(MissileManager *mi_mgr, LockOnSystem *lockon_sys, UltimateManager *ult, XMFLOAT3 pos)
{
	is_dead_ = false;
	hp_ = 3;

	// 他クラス情報を格納
	p_mi_mgr_ = mi_mgr;
	p_lockon_sys_ = lockon_sys;
	p_ult_ = ult;

	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(pos);
	obj_->SetRot(XMFLOAT3(0, 180.0f, 0));

	// コリジョンの更新
	UpdateColl();

	// イージング関連の初期化
	NcmEaseDesc args;
	args.init_value = 0.0f;
	args.total_move = -rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = 0.05f;
	ease_rot_right_ = NcmEasing::RegisterEaseData(args);

	args.init_value = 0.0f;
	args.total_move = rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = 0.05f;
	ease_rot_left_ = NcmEasing::RegisterEaseData(args);

	args.init_value = 0.0f;
	args.total_move = rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = 0.05f;
	ease_reset_rot_ = NcmEasing::RegisterEaseData(args);
}

void Player::Finalize()
{}

void Player::Update()
{
	// 死んだら
	if (is_dead_)
	{
		// 以降の処理をスルー
		return;
	}

	// スペース長押しで
	if (KeyboardInput::HoldKey(DIK_SPACE))
	{
		// ミサイルをチャージ
		ChargeMissile();
	}
	// 離して
	else if (KeyboardInput::ReleaseKey(DIK_SPACE))
	{
		// ミサイルを発射
		FireChargeMissile();
		p_lockon_sys_->ResetTargetNum();
		count_ = 0;
	}

	// Qで
	if (KeyboardInput::ReleaseKey(DIK_Q))
	{
		// ウルトが溜まっていなかったら
		if (!p_ult_->NoticeFullCharged())
		{
			// チートモードなら
			if (NcmDebug::GetInstance()->IsCheatMode())
			{
				// 強制的に発動する
				p_ult_->TriggeringUlt();
			}
			else
			{
				return;
			}
		}

		// ウルトを発動する
		p_ult_->TriggeringUlt();
		FireUltimateMissile();
		is_triggering_ult_ = true;
	}

	// ULTの発動を検知したら
	if (is_triggering_ult_)
	{
		// ULT用ミサイルセットを発射する
		FireUltimateMissile();
	}

	// HPが0以下なら
	if (IsZeroOrLess(hp_))
	{
		// 死亡
		is_dead_ = true;
	}

	// ダメージを受けたら
	if (taking_damage_trigger_)
	{
		// 無敵にする
		is_invincible_ = true;
		taking_damage_trigger_ = false;
	}

	// 無敵なら
	if (is_invincible_)
	{
		// 残り無敵時間を更新する
		CountInvincibleTime();
	}

	// 入力により回頭させる
	RotationY(1.0f);

	// 自動で前に進む
	MoveForwardAuto();

	obj_->Update();
	UpdateColl();
	CalcFwdVec();
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

void Player::FireMultiMissile()
{
	MissileArgs l_args{};
	l_args.pos = obj_->GetPos();
	l_args.vel = XMFLOAT3(0, 0, 1.0f);
	//l_args.acc = XMFLOAT3(0, -1.0f, 0);
	// acc はMissileManagerで設定
	// tgt_pos はMissileManagerで設定
	// tgt_index はMissileManagerで設定
	l_args.detection_range = 1000.0f;
	l_args.init_straight_time_ = 0;
	l_args.life = 200;

	p_mi_mgr_->FireMultiMissile(l_args, 4);
}

void Player::FireChargeMissile()
{
	MissileArgs l_args{};
	l_args.pos = obj_->GetPos();
	l_args.vel = XMFLOAT3(0, 0, 1.0f);
	//l_args.acc = XMFLOAT3(0, -1.0f, 0);
	// acc はMissileManagerで設定
	// tgt_pos はMissileManagerで設定
	// tgt_index はMissileManagerで設定
	l_args.detection_range = 1000.0f;
	l_args.init_straight_time_ = 0;
	l_args.life = 200;

	p_mi_mgr_->FireChargeMissile(l_args);
}

void Player::FireUltimateMissile()
{
	static const uint32_t DELAY = 3;
	static uint32_t count = 1;
	static uint32_t launched = 0;
	count--;

	if (IsZeroOrLess(count))
	{
		MissileArgs l_args{};
		l_args.pos = obj_->GetPos();
		l_args.vel = XMFLOAT3(0, 0, 1.0f);
		//l_args.acc = XMFLOAT3(0, -1.0f, 0);
		// acc はMissileManagerで設定
		// tgt_pos はMissileManagerで設定
		// tgt_index はMissileManagerで設定
		l_args.detection_range = 1000.0f;
		l_args.init_straight_time_ = 0;
		l_args.life = 200;

		launched++;

		p_mi_mgr_->FireUltimateMissile(l_args, launched);
		count = DELAY;
	}

	if (launched >= 20)
	{
		is_triggering_ult_ = false;
		launched = 0;
		count = 1;
	}
}

void Player::ChargeMissile()
{
	if (p_lockon_sys_->GetCurrentTgtNum() >= p_lockon_sys_->GetMaxTgtNum())
	{
		return;
	}

	count_++;

	if (count_ >= charge_time_)
	{
		p_lockon_sys_->AddTargetNum();
		count_ = 0;
	}
}

void Player::TakeDamage()
{
	if (is_invincible_ || NcmDebug::GetInstance()->IsCheatMode())
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
		//RotPoseRight();
		is_released = false;
	}
	else if (KeyboardInput::PushKey(DIK_A))
	{
		pos.x -= speed;
		//RotPoseLeft();
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
		//ResetRotPose();
	}
	else
	{
		NcmEasing::ResetTime(ease_reset_rot_);
	}
}

void Player::RotationY(float speed)
{
	// 回頭処理
	XMFLOAT3 rot = obj_->GetRot();

	if (KeyboardInput::PushKey(DIK_A))
	{
		rot.y -= speed;
		RotPoseRight(rot);
		is_released = false;
	}
	else if (KeyboardInput::PushKey(DIK_D))
	{
		rot.y += speed;
		RotPoseLeft(rot);
		is_released = false;
	}

	if (KeyboardInput::ReleaseKey(DIK_A) || KeyboardInput::ReleaseKey(DIK_D))
	{
		is_released = true;
		NcmEasing::ResetTime(ease_rot_right_);
		NcmEasing::ResetTime(ease_rot_left_);
	}

	if (is_released)
	{
		ResetRotPose(rot);
	}
	else
	{
		NcmEasing::ResetTime(ease_reset_rot_);
	}

	obj_->SetRot(rot);
}

void Player::MoveForwardAuto()
{
	XMFLOAT3 pos = obj_->GetPos();
	pos.x += speed_ * forward_vec_.x;
	pos.y += speed_ * forward_vec_.y;
	pos.z += speed_ * forward_vec_.z;
	obj_->SetPos(pos);
}

void Player::RotPoseLeft(XMFLOAT3 &rot)
{
	is_already_ = false;
	NcmEasing::UpdateValue(ease_rot_left_);
	rot.z = NcmEasing::GetValue(ease_rot_left_);
	obj_->SetRot(rot);
}

void Player::RotPoseRight(XMFLOAT3 &rot)
{
	is_already_ = false;
	NcmEasing::UpdateValue(ease_rot_right_);
	rot.z = NcmEasing::GetValue(ease_rot_right_);
	obj_->SetRot(rot);
}

void Player::ResetRotPose(XMFLOAT3 &rot)
{
	if (!is_already_)
	{
		float rot = obj_->GetRot().z;
		NcmEasing::SetInitValue(ease_reset_rot_, rot);

		// それが正なら
		if (IsPlus(rot))
		{
			NcmEasing::SetTotalMove(ease_reset_rot_, -rot_angle_);
		}
		// それが負なら
		else if (IsMinus(rot))
		{
			NcmEasing::SetTotalMove(ease_reset_rot_, rot_angle_);
		}

		is_already_ = true;
	}

	NcmEasing::UpdateValue(ease_reset_rot_);
	rot.z = NcmEasing::GetValue(ease_reset_rot_);
	obj_->SetRot(rot);
}
