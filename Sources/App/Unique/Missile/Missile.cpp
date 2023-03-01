#include "Missile.h"
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Sources/Lib/NacamLib/NacamLib.h"
#include "../../../Lib/NacamError/NacamError.h"
#include "../Sources/App/Math/NcmMath.h"
#include "../../../Lib/PreDraw/PreDraw.h"
#include "../Player/Player.h"
#include "../../Utility/NcmColor.h"

using namespace DirectX;
using namespace NcmUtill;
using namespace NcmMath;
using namespace NcmColor;

std::unique_ptr<Model> Missile::model_ = nullptr;
std::unique_ptr<Model> Missile::coll_model_ = nullptr;

Missile::Missile()
	: AbsUniqueObj(3.0f, 1.0f),
	homing_sequence_(std::make_unique<MissileHoming>()),
	emitter_(std::make_unique<Emitter>()),
	explo_emi_(std::make_unique<Emitter>()),
	mi_param_()
{}

Missile::~Missile()
{}

void Missile::LoadResources()
{
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

void Missile::Initialize(const MissileParam &args)
{
	// �l����͂��ݒ�
	mi_param_ = args;
	speed_ = mi_param_.speed;

	// �`��f�[�^��������
	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPos(XMFLOAT3(mi_param_.pos));

	/* �~�T�C���O�Ղ̃G�~�b�^�[ */
	EmitterDesc emi{};
	emi.part_desc_.position_ = mi_param_.pos;
	// �G�~�b�^�[�p�Ƀ~�T�C���̑��x�𔽓]���Ē�
	XMFLOAT3 temp = mi_param_.vel;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.part_desc_.velocity_ = temp;
	// �G�~�b�^�[�p�Ƀ~�T�C���̉����x�𔽓]���Ē�
	temp = mi_param_.acc;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.part_desc_.accel_ = temp;
	emi.part_desc_.life_ = mi_param_.life;
	emi.part_desc_.s_scale_ = 1.5f;
	emi.part_desc_.color_ = mi_param_.trail_color;
	emi.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	emi.vel_rand_ = { 0.01f, 0.01f, 0.01f };
	emi.gene_num_ = 1;
	emi.use_life_ = true;
	emi.life_ = mi_param_.life;
	emitter_->SetEmitterDesc(emi);
}

void Missile::Initialize()
{}

void Missile::Finalize()
{}

void Missile::Update()
{
	// �������s������
	if (IsZeroOrLess(mi_param_.life))
	{
		// �~�T�C���𖳌��ɂ���
		InvalidateMissile();

		// �G�~�b�^�[�̏I���������o������
		if (emitter_->NoticeCanTerminate() &&
			explo_emi_->NoticeCanTerminate())
		{
			// �E��
			is_dead_ = true;
			return;
		}
	}

	// �܂�����������Ȃ�
	if (!IsZero(mi_param_.life))
	{
		// ���������炷
		mi_param_.life--;
	}

	// �~�T�C�����L���Ȃ�
	if (mi_param_.is_validity)
	{
		// �X�V����������
		obj_->Update();
		UpdateColl();
	}

	// �������Ȃ�
	if (mi_param_.is_explode)
	{
		explo_emi_->UpdateParticle();
	}

	UpdateEmitter();
	CalcFwdVec();
}

void Missile::Draw()
{
	// �~�T�C�����L���Ȃ�~�T�C����`��
	PreDraw::SetPipeline(PipelineName::Object3d);
	if (mi_param_.is_validity) { obj_->Draw(); }

	// �p�[�e�B�N����`��
	emitter_->Draw();
}

void Missile::DrawColl()
{
	if (mi_param_.is_validity) { coll_obj_->Draw(); }
}

void Missile::DebugDraw()
{
	ImGui::Text("emitter_pos : (%f, %f, %f)", explo_emi_->GetPosition().x, explo_emi_->GetPosition().y, explo_emi_->GetPosition().z);
	ImGui::Checkbox("validity", &mi_param_.is_validity);
	ImGui::Text("pos : (%f, %f, %f)", mi_param_.pos.x, mi_param_.pos.y, mi_param_.pos.z);
	ImGui::Text("vel : (%f, %f, %f)", mi_param_.vel.x, mi_param_.vel.y, mi_param_.vel.z);
	ImGui::Text("acc : (%f, %f, %f)", mi_param_.acc.x, mi_param_.acc.y, mi_param_.acc.z);
	ImGui::Text("alpha : %f", emitter_->GetEmitterDesc().part_desc_.alpha_);
}

void Missile::Activate()
{
	// ���S�t���O���~�낷
	is_dead_ = false;

	// �~�T�C����L����
	mi_param_.is_validity = true;
}

void Missile::InvalidateMissile()
{
	if (!mi_param_.is_validity)
	{
		return;
	}

	// �~�T�C���𖳌���
	mi_param_.is_validity = false;

	// ����������
	ExplosionOnDeath();
}

void Missile::ExplosionOnDeath()
{
	// �����t���O�����Ă�
	mi_param_.is_explode = true;

	/* �����̃G�~�b�^�[ */
	EmitterDesc explo_emi{};
	explo_emi.part_desc_.position_ = mi_param_.pos;
	XMVECTOR norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_param_.vel));
	explo_emi.part_desc_.velocity_.x = norm_vec.m128_f32[0] * 0.1f;
	explo_emi.part_desc_.velocity_.y = norm_vec.m128_f32[1] * 0.1f;
	explo_emi.part_desc_.velocity_.z = norm_vec.m128_f32[2] * 0.1f;
	explo_emi.part_desc_.accel_ = XMFLOAT3(0, 0, 0);
	explo_emi.part_desc_.life_ = 50;
	explo_emi.part_desc_.s_scale_ = 2.0f;
	explo_emi.part_desc_.e_scale_ = 0;
	explo_emi.part_desc_.color_ = NcmColor::EXPLOSION_ORENGE;
	explo_emi.part_desc_.alpha_ = 1.0f;
	explo_emi.pos_rand_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	explo_emi.vel_rand_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
	explo_emi.gene_num_ = EXPLO_PARTICLE_NUM_;
	explo_emi.use_life_ = false;
	explo_emi_->SetEmitterDesc(explo_emi);

	// �p�[�e�B�N���𐶐�
	explo_emi_->GenerateParticle();
}

void Missile::MoveZ(float speed)
{
	XMFLOAT3 pos = obj_->GetPos();
	pos.z += mi_param_.vel.z;
	obj_->SetPos(pos);
}

void Missile::UpdateTargetPos(EnemiesList &enemies)
{
	// �ǔ��������^�[�Q�b�g��ID�������Ă���v�f�̓Y��������
	int32_t index = enemies.GetEnemyIndexWithID(mi_param_.tgt_id);

	// ���Ȃ�������
	if (index == (int32_t)(NacamError::NonDetected))
	{
		InvalidateMissile();
		return;
	}
	// �^�[�Q�b�g���v���C���[�Ȃ�
	else if (index == (int32_t)(TargetIs::Player))
	{
		mi_param_.tgt_pos = p_player_->GetPos();
		return;
	}

	// ���������Y�������ƂɃ^�[�Q�b�g�̈ʒu�����
	mi_param_.tgt_pos = enemies.GetPos(index);
}

void Missile::HomingTarget()
{
	homing_sequence_->HomingTarget(*this, HomingAccuracy::High);
}

void Missile::PrepareTermEmitter()
{
	emitter_->PrepareTerminate();
}

void Missile::UpdateEmitter()
{
	emitter_->SetPosition(obj_->GetPos());
	emitter_->GenerateParticle();
	emitter_->UpdateParticle();
}
