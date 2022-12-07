#include "Player.h"
#include "../../Utility/NcmUtil.h"
#include "../../Debug/NcmImGui.h"
#include "../../Math/Easing/NcmEasing.h"
#include "../../Debug/NcmDebug.h"
#include "../../../Lib/Input/KeyboardInput.h"
#include "../Sources/App/Math/NcmMath.h"
#include "../../../Lib/Input/NcmInput.h"

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
	mi_launcher_(std::make_unique<MissileLauncher>()),
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
	if (KeyboardInput::HoldKey(DIK_SPACE) || NcmInput::IsPush(NcmButtonType::A))
	{
		// ミサイルをチャージ
		ChargeMissile();
	}
	// 離して
	else if (KeyboardInput::ReleaseKey(DIK_SPACE) || NcmInput::IsRelease(NcmButtonType::A))
	{
		// ミサイルを発射
		mi_launcher_->FireMissile(MissileType::Charge, obj_->GetPos(), p_mi_mgr_);

		p_lockon_sys_->ResetTargetNum();
		count_ = 0;
	}

	// Qで
	if (KeyboardInput::TriggerKey(DIK_Q) || NcmInput::IsTrigger(NcmButtonType::Y))
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

		// ウルトを発動する(UI)
		p_ult_->TriggeringUlt();
		is_triggering_ult_ = true;
	}

	// ULTの発動を検知したら
	if (is_triggering_ult_)
	{
		// ULT用ミサイルセットを発射する
		if (mi_launcher_->FireMissile(MissileType::Ultimate, obj_->GetPos(), p_mi_mgr_))
		{
			is_triggering_ult_ = false;
		}
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
	//MoveForwardAuto();

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
	using enum NcmStickType;
	using enum NcmStickDirection;

	// 回頭処理
	XMFLOAT3 rot = obj_->GetRot();

	if (KeyboardInput::PushKey(DIK_A) || NcmInput::IsHold(L_STICK, LEFT))
	{
		rot.y -= speed;
		RotPoseRight(rot);
		is_released = false;
	}
	else if (KeyboardInput::PushKey(DIK_D) || NcmInput::IsHold(L_STICK, RIGHT))
	{
		rot.y += speed;
		RotPoseLeft(rot);
		is_released = false;
	}

	if (KeyboardInput::ReleaseKey(DIK_A) || KeyboardInput::ReleaseKey(DIK_D) ||
		NcmInput::IsRelease(L_STICK, LEFT) || NcmInput::IsRelease(L_STICK, RIGHT))
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
		if (IsZeroOrMore(rot))
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
