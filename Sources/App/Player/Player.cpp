#include "Player.h"
#include "../../Lib/Utility/NcmUtil.h"
#include "../../Lib/DebugUtill/NcmImGui.h"
#include "../../Lib/Math/Easing/NcmEasing.h"
#include "../../Lib/DebugUtill/NcmDebug.h"
#include "../../Lib/Input/KeyboardInput.h"
#include "../../Lib/Math/NcmMath.h"
#include "../../Lib/Input/NcmInput.h"
#include "../../Lib/Camera/Camera.h"
#include "../../Lib/PostEffect/PostEffect.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace NcmUtill;
using namespace NcmMath;

std::unique_ptr<Model> Player::model_ = nullptr;
std::unique_ptr<Model> Player::coll_model_ = nullptr;
Camera *Player::cam_ptr_ = nullptr;

Player::Player()
	: AbsUniqueObj(3.0f, 2.0f),
	normal_speed_(),
	is_invincible_(false),
	taking_damage_trigger_(false),
	is_triggering_ult_(false),
	hp_(),
	p_lockon_sys_(nullptr),
	p_ult_(nullptr),
	mi_launcher_(std::make_unique<MissileLauncher>()),
	charge_time_(40),
	count_(),
	ease_rot_right_(),
	ease_rot_left_(),
	ease_reset_rot_(),
	ease_move_left_(),
	ease_move_right_(),
	ease_move_reset_(),
	acc_speed_(),
	dec_speed_(),
	fov_acc_value_(),
	fov_dec_value_(),
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

	// �R���W�����̍X�V
	UpdateColl();

	// �C�[�W���O�֘A�̏�����
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

void Player::Initialize(LockOnSystem *lockon_sys, UltimateManager *ult, const XMFLOAT3 &pos)
{
	is_dead_ = false;
	hp_ = 3;

	// ���N���X�����i�[
	p_lockon_sys_ = lockon_sys;
	p_ult_ = ult;

	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(pos);
	obj_->SetRot(XMFLOAT3(0, 180.0f, 0));

	// �R���W�����̍X�V
	UpdateColl();

	// �ʏ푬�x��ۑ�
	normal_speed_ = speed_;

	// �C�[�W���O�֘A�̏�����
	float rate = 0.05f;

	// �E�ړ����̉�ʌX�|�C�[�W���O
	NcmEaseDesc args;
	args.init_value = 0.0f;
	args.total_move = -rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = rate;
	ease_rot_right_ = NcmEasing::RegisterEaseData(args);

	// ���ړ����̉�ʌX�|�C�[�W���O
	args.init_value = 0.0f;
	args.total_move = rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = rate;
	ease_rot_left_ = NcmEasing::RegisterEaseData(args);

	// �X�|���Z�b�g�C�[�W���O
	args.init_value = 0.0f;
	args.total_move = rot_angle_;
	args.ease_type = NcmEaseType::OutCubic;
	args.t_rate = rate;
	ease_reset_rot_ = NcmEasing::RegisterEaseData(args);

	// �����C�[�W���O
	args.ease_type = NcmEaseType::OutCirc;
	args.init_value = 0.5f;
	args.total_move = normal_speed_;
	//ease.total_move = SPEED_;
	args.t_rate = rate;
	acc_speed_ = NcmEasing::RegisterEaseData(args);

	// �����C�[�W���O
	args.ease_type = NcmEaseType::OutCirc;
	args.init_value = normal_speed_;
	args.total_move = -0.5f;
	args.t_rate = rate;
	dec_speed_ = NcmEasing::RegisterEaseData(args);

	args.ease_type = NcmEaseType::OutCubic;
	args.init_value = NORMAL_FOV_;
	args.total_move = ACCEL_FOV_ - NORMAL_FOV_;
	args.t_rate = rate;
	fov_acc_value_ = NcmEasing::RegisterEaseData(args);

	args.ease_type = NcmEaseType::OutCubic;
	args.init_value = ACCEL_FOV_;
	args.total_move = NORMAL_FOV_ - ACCEL_FOV_;
	args.t_rate = rate;
	fov_dec_value_ = NcmEasing::RegisterEaseData(args);
}

void Player::Finalize()
{}

void Player::Update()
{
	// ���񂾂�
	if (is_dead_)
	{
		// �ȍ~�̏������X���[
		return;
	}

	// �X�y�[�X��������
	if (KeyboardInput::HoldKey(DIK_SPACE) || NcmInput::IsPush(NcmButtonType::A))
	{
		// �~�T�C�����`���[�W
		ChargeMissile();
	}
	// ������
	else if (KeyboardInput::ReleaseKey(DIK_SPACE) || NcmInput::IsRelease(NcmButtonType::A))
	{
		// �~�T�C���𔭎�
		mi_launcher_->FireMissile(MissileType::Charge, LaunchedBy::Player, GetPos());

		p_lockon_sys_->ResetTargetNum();
		count_ = 0;
	}

	// Q��
	if (KeyboardInput::TriggerKey(DIK_Q) || NcmInput::IsTrigger(NcmButtonType::Y))
	{
		// �E���g�����܂��Ă��Ȃ�������
		if (!p_ult_->NoticeFullCharged())
		{
			// �`�[�g���[�h�Ȃ�
			if (NcmDebug::GetInstance()->IsCheatMode())
			{
				// �����I�ɔ�������
				UltSequence();
			}
			else
			{
				return;
			}
		}
		else
		{
			// �E���g�𔭓�����
			UltSequence();
		}
	}

	// �~�T�C�������m�F�p
	if (NcmDebug::GetInstance()->IsCheatMode())
	{
		if (KeyboardInput::TriggerKey(DIK_E))
		{
			mi_launcher_->FireMissile(MissileType::Mono, LaunchedBy::Player, GetPos());
		}
	}

	// ��������
	Accelerate();

	// ULT�̔��������m������
	if (is_triggering_ult_)
	{
		// ULT�p�~�T�C���Z�b�g�𔭎˂���
		if (mi_launcher_->FireMissile(MissileType::Ultimate, LaunchedBy::Player, GetPos()))
		{
			is_triggering_ult_ = false;
			cam_ptr_->SetIsMissileCamera(false);
		}
	}

	// HP��0�ȉ��Ȃ�
	if (IsZeroOrLess(hp_))
	{
		// ���S
		is_dead_ = true;
	}

	// �_���[�W���󂯂���
	if (taking_damage_trigger_)
	{
		// ���G�ɂ���
		is_invincible_ = true;
		taking_damage_trigger_ = false;
	}

	// ���G�Ȃ�
	if (is_invincible_)
	{
		// �c�薳�G���Ԃ��X�V����
		CountInvincibleTime();
	}

	// �㉺���E�ړ�
	MoveXY(1.0f);

	// �����őO�ɐi��
	MoveZ(speed_);

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
	// ���^�[�Q�b�g�����ő�^�[�Q�b�g���𒴂�����
	if (p_lockon_sys_->GetCurrentTgtNum() >= p_lockon_sys_->GetMaxTgtNum())
	{
		// �ȍ~�̏������X���[
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
	p_ult_->AddUltValue(30);
}

void Player::CountInvincibleTime()
{
	static uint32_t count = INVINCIBLE_TIME_;
	count--;

	if (IsZeroOrLess(count))
	{
		is_invincible_ = false;
		count = INVINCIBLE_TIME_;
	}
}

void Player::UltSequence()
{
	p_ult_->TriggeringUlt();
	is_triggering_ult_ = true;
	cam_ptr_->SetIsMissileCamera(true);
}

void Player::MoveXZ(float speed)
{
	// ���݂̈ʒu���擾
	XMFLOAT3 pos = obj_->GetPos();

	// �O��ړ�
	if (KeyboardInput::PushKey(DIK_W)) { pos.z += speed; }
	else if (KeyboardInput::PushKey(DIK_S)) { pos.z -= speed; }

	// ���E�ړ�
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

	// �ʒu���X�V
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

void Player::MoveXY(float speed)
{
	// ���݂̈ʒu���擾
	XMFLOAT3 pos = obj_->GetPos();
	XMFLOAT3 rot = obj_->GetRot();

	// �㉺�ړ�
	if (KeyboardInput::PushKey(DIK_W) && pos.y <= MOVABLE_RANGE_.y) { pos.y += speed; }
	if (KeyboardInput::PushKey(DIK_S) && pos.y >= 0.0f) { pos.y -= speed; }

	// ���E�ړ�
	if (KeyboardInput::PushKey(DIK_D) && pos.x <= MOVABLE_RANGE_.x)
	{
		pos.x += speed;
		RotPoseRight(rot);
		is_released = false;
	}
	if (KeyboardInput::PushKey(DIK_A) && pos.x >= -MOVABLE_RANGE_.x)
	{
		pos.x -= speed;
		RotPoseLeft(rot);
		is_released = false;
	}

	// �ʒu���X�V
	obj_->SetPos(pos);

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
}

void Player::RotationY(float speed)
{
	using enum NcmStickType;
	using enum NcmStickDirection;

	// �񓪏���
	XMFLOAT3 rot = obj_->GetRot();

	if (KeyboardInput::PushKey(DIK_A) || NcmInput::IsHold(L_STICK, LEFT))
	{
		rot.y -= speed;
		RotPoseRight(rot);
		is_released = false;
	}
	if (KeyboardInput::PushKey(DIK_D) || NcmInput::IsHold(L_STICK, RIGHT))
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

volatile void Player::MoveForwardAuto(float speed)
{
	XMFLOAT3 pos = obj_->GetPos();
	pos.x += speed * forward_vec_.x;
	pos.y += speed * forward_vec_.y;
	pos.z += speed * forward_vec_.z;
	obj_->SetPos(pos);
}

void Player::MoveZ(float speed)
{
	XMFLOAT3 pos = obj_->GetPos();
	pos.z += speed;
	obj_->SetPos(pos);
}

void Player::Accelerate()
{
	// ����
	if (KeyboardInput::PushKey(DIK_LSHIFT))
	{
		NcmEasing::ResetTime(fov_dec_value_);
		NcmEasing::ResetTime(dec_speed_);

		// �C�[�W���O�̒l��J��
		NcmEasing::UpdateValue(acc_speed_);
		NcmEasing::UpdateValue(fov_acc_value_);

		float fov = cam_ptr_->GetFOV();
		fov = NcmEasing::GetValue(fov_acc_value_);
		cam_ptr_->SetFOV(fov);

		SetSpeed(NcmEasing::GetValue(acc_speed_));

		PostEffect::ChangePipeline(Pipeline::RadialBlur);
	}
	// �����E�ʏ�
	else
	{
		NcmEasing::ResetTime(acc_speed_);
		NcmEasing::ResetTime(fov_acc_value_);

		// �C�[�W���O�̒l��J��
		NcmEasing::UpdateValue(dec_speed_);
		NcmEasing::UpdateValue(fov_dec_value_);

		float fov = cam_ptr_->GetFOV();
		fov = NcmEasing::GetValue(fov_dec_value_);
		cam_ptr_->SetFOV(fov);

		SetSpeed(NcmEasing::GetValue(dec_speed_));

		PostEffect::ChangePipeline(Pipeline::Basic);
	}
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

		// ���ꂪ���Ȃ�
		if (IsZeroOrMore(rot))
		{
			NcmEasing::SetTotalMove(ease_reset_rot_, -rot_angle_);
		}
		// ���ꂪ���Ȃ�
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
