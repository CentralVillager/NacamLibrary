#include "Missile.h"
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Sources/Lib/NacamLib/NacamLib.h"
#include "../../../Lib/NacamError/NacamError.h"

using namespace DirectX;
using namespace NcmUtill;

std::unique_ptr<Model> Missile::model_ = nullptr;
std::unique_ptr<Model> Missile::coll_model_ = nullptr;

Missile::Missile()
	: AbsUniqueObj(1.0f, 1.0f),
	emitter_(std::make_unique<Emitter>()),
	mi_args_(),
	min_dist_(),
	rot_dead_zone_(),
	tgt_index_(),
	lockon_sys_(nullptr)
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

void Missile::Initialize(const MissileArgs &args, LockOnSystem *sys)
{
	lockon_sys_ = sys;

	// �l����͂��ݒ�
	mi_args_ = args;

	InitObj3d(model_.get(), coll_model_.get());
	obj_->SetPosition(XMFLOAT3(mi_args_.pos));
	obj_->SetRotation(XMFLOAT3(0, 180.0f, 0));

	Emitter::EmitterArgs emi;
	XMFLOAT3 temp = mi_args_.vel;
	temp.x -= temp.x;
	temp.y -= temp.y;
	temp.z -= temp.z;
	emi.particle.position_ = mi_args_.pos;
	emi.particle.velocity_ = temp;
	emi.particle.accel_ = { 0, 0, 0 };
	emi.particle.life_ = mi_args_.life;
	emi.particle.s_scale_ = 1.0f;
	emi.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	emi.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	emi.gene_num_ = 1;
	emi.use_life_ = true;
	emi.life_ = mi_args_.life;
	emitter_->SetEmitterArgs(emi);
}

void Missile::Initialize()
{}

void Missile::Finalize()
{}

void Missile::Update()
{
	// ���R����
	// �������s���� && �G�~�b�^�[�̏I���������o����
	if (mi_args_.life <= 0 && emitter_->NoticeCanTerminate())
	{
		// �E��
		is_dead_ = true;
		return;
	}

	if (mi_args_.life > 0)
	{
		mi_args_.life--;
	}

	// �~�T�C�����L���Ȃ�
	if (mi_args_.is_validity)
	{
		// �X�V����������
		obj_->Update();
		UpdateColl();
	}

	UpdateEmitter();
}

void Missile::Draw()
{
	if (mi_args_.is_validity) { obj_->Draw(); }
	emitter_->Draw();
}

void Missile::DrawColl()
{
	if (mi_args_.is_validity) { coll_obj_->Draw(); }
}

void Missile::DebugDraw()
{}

void Missile::Activate()
{
	is_dead_ = false;
	mi_args_.is_validity = true;
}

void Missile::MoveZ(float speed)
{
	XMFLOAT3 pos = obj_->GetPosition();
	pos.z += mi_args_.vel.z;
	obj_->SetPosition(pos);
}

void Missile::HomingTarget(const XMFLOAT3 &target_pos)
{
	XMFLOAT3 pos = obj_->GetPosition();

	static XMVECTOR old_len{};
	const float speed = 3.0f;	// ��

	if (mi_args_.init_straight_time_ > 0)
	{
		mi_args_.init_straight_time_--;
		pos.z += speed;
		obj_->SetPosition(pos);

		return;
	}

	// XMVECTOR�ɕϊ�
	XMVECTOR mi_vec = XMLoadFloat3(&obj_->GetPosition());
	XMVECTOR ta_vec = XMLoadFloat3(&mi_args_.tgt_pos);
	//XMVECTOR tgt_vec = XMLoadFloat3(&target_pos);

	// �ӂ��̍��W�����ԃx�N�g�����v�Z
	XMVECTOR vec =
	{
		(ta_vec.m128_f32[0] - mi_vec.m128_f32[0]),
		(ta_vec.m128_f32[1] - mi_vec.m128_f32[1]),
		(ta_vec.m128_f32[2] - mi_vec.m128_f32[2])
	};

	XMVECTOR len = XMVector3Length(vec);
	min_dist_ = len.m128_f32[0];

	// �~�T�C���ƖڕW������n�߂���
	//if (len.m128_f32[0] >= old_len.m128_f32[0]) {

	//	// �ێ����Ă��鑬�x�����Z
	//	pos.x += mi_args_.vel.x;
	//	pos.y += mi_args_.vel.y;
	//	pos.z += mi_args_.vel.z;

	//	// �ʒu�𔽉f
	//	object_->SetPosition(pos);

	//	// ���̌�̒ǔ��������X�L�b�v
	//	return;
	//}

	// �ǔ��͈͊O�Ȃ�
	if (len.m128_f32[0] >= mi_args_.detection_range)
	{
		// ���i��������
		pos.z += speed;
		obj_->SetPosition(pos);

		// ���̌�̒ǔ��������X�L�b�v
		return;
	}

	//if (pos.z >= target_pos.z)
	if (pos.z >= mi_args_.tgt_pos.z)
	{
		// �ʒu���X�V
		pos.x += mi_args_.vel.x;
		pos.y += mi_args_.vel.y;
		pos.z += mi_args_.vel.z;

		// �ʒu�𔽉f
		obj_->SetPosition(pos);

		return;
	}

	// ���K��
	XMVECTOR norm_vec = XMVector3Normalize(vec);
	XMVECTOR mi_norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_args_.vel));

	rot_dead_zone_ = 1.0f;

	XMVECTOR differ = {

		norm_vec.m128_f32[0] - mi_norm_vec.m128_f32[0],
		norm_vec.m128_f32[1] - mi_norm_vec.m128_f32[1],
		norm_vec.m128_f32[2] - mi_norm_vec.m128_f32[2]
	};

	// ���x�����Z
	/*norm_vec.m128_f32[0] *= speed;
	norm_vec.m128_f32[1] *= speed;
	norm_vec.m128_f32[2] *= speed;*/

	differ.m128_f32[0] /= rot_dead_zone_;
	differ.m128_f32[1] /= rot_dead_zone_;
	differ.m128_f32[2] /= rot_dead_zone_;

	/*mi_args_.vel.x += norm_vec.m128_f32[0];
	mi_args_.vel.y += norm_vec.m128_f32[1];
	mi_args_.vel.z += norm_vec.m128_f32[2];*/
	mi_args_.vel.x += differ.m128_f32[0] * speed;
	mi_args_.vel.y += differ.m128_f32[1] * speed;
	mi_args_.vel.z = speed;

	// �ʒu���X�V
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;

	// �ʒu�𔽉f
	obj_->SetPosition(pos);

	/* -- �O�ϒǔ� -- */
	// XMVECTOR�ɕϊ�
	//XMVECTOR mi_vec = XMLoadFloat3(&object_->GetPosition());
	//XMVECTOR tgt_vec = XMLoadFloat3(&target_pos);

	//// �ӂ��̍��W�����ԃx�N�g�����v�Z
	//XMVECTOR vec = {
	//	(tgt_vec.m128_f32[0] - mi_vec.m128_f32[0]),
	//	(tgt_vec.m128_f32[1] - mi_vec.m128_f32[1]),
	//	(tgt_vec.m128_f32[2] - mi_vec.m128_f32[2])
	//};

	//// �������v�Z
	//XMVECTOR len = XMVector3Length(vec);

	//// ���K��
	//XMVECTOR norm_vec = XMVector3Normalize(vec);

	//float r = vec.m128_f32[0] * mi_vec.m128_f32[2] - mi_vec.m128_f32[0] * vec.m128_f32[2];

	//if (IsPlus(r)) {

	//	norm_vec.m128_f32[0] += 1.0f;

	//} else if (IsMinus(r)) {

	//	norm_vec.m128_f32[0] += -1.0f;
	//}

	//pos.x += norm_vec.m128_f32[0];
	//pos.y += norm_vec.m128_f32[1];
	//pos.z += norm_vec.m128_f32[2];

	//// �ʒu�𔽉f
	//object_->SetPosition(pos);
}

void Missile::HomingTarget(EnemiesList &enemies)
{
	const float speed = 3.0f;	// ��
	XMFLOAT3 pos = obj_->GetPosition();

	// �G�����Ȃ��Ȃ�
	if (enemies.GetEnemies().size() <= 0)
	{
		// ���i������
		MoveZ(speed);

		return;
	}

	// ���ǔ����Ԓ��Ȃ�
	if (mi_args_.init_straight_time_ > 0)
	{
		mi_args_.init_straight_time_--;

		// ���i������
		MoveZ(speed);

		return;
	}

	// XMVECTOR�ɕϊ�
	XMVECTOR mi_vec = XMLoadFloat3(&obj_->GetPosition());

	// �ǔ��������G��ID�������Ă���v�f�̓Y��������
	int index = enemies.GetEnemyIndexWithID(mi_args_.tgt_id);

	// ���Ȃ�������
	if (index == (int)(NacamError::NonDetected))
	{
		// �ȑO�̑��x�����̂܂܉��Z
		pos.x += mi_args_.vel.x;
		pos.y += mi_args_.vel.y;
		pos.z += mi_args_.vel.z;

		// �ʒu�𔽉f
		obj_->SetPosition(pos);

		// ���̌�̏������X�L�b�v
		return;
	}

	// ���������Y�������ƂɓG�̈ʒu�����
	XMVECTOR tgt_vec = XMLoadFloat3(&enemies.GetPos(index));

	// �ӂ��̍��W�����ԃx�N�g�����v�Z
	XMVECTOR vec =
	{
		(tgt_vec.m128_f32[0] - mi_vec.m128_f32[0]),
		(tgt_vec.m128_f32[1] - mi_vec.m128_f32[1]),
		(tgt_vec.m128_f32[2] - mi_vec.m128_f32[2])
	};

	XMVECTOR len = XMVector3Length(vec);
	min_dist_ = len.m128_f32[0];

	// �ǔ��͈͊O�Ȃ�
	if (len.m128_f32[0] >= mi_args_.detection_range)
	{
		// ���i��������
		MoveZ(speed);

		// ���̌�̒ǔ��������X�L�b�v
		return;
	}

	// �~�T�C�����G��ǂ��z������
	if (pos.z >= enemies.GetPos(index).z)
	{
		// �ȑO�̑��x�����̂܂܉��Z
		pos.x += mi_args_.vel.x;
		pos.y += mi_args_.vel.y;
		pos.z += mi_args_.vel.z;

		// �ʒu�𔽉f
		obj_->SetPosition(pos);

		return;
	}

	// ���K��
	XMVECTOR norm_vec = XMVector3Normalize(vec);
	XMVECTOR mi_norm_vec = XMVector3Normalize(XMLoadFloat3(&mi_args_.vel));

	rot_dead_zone_ = 1.0f;

	// ����Ă��鋗��
	XMVECTOR differ =
	{
		norm_vec.m128_f32[0] - mi_norm_vec.m128_f32[0],
		norm_vec.m128_f32[1] - mi_norm_vec.m128_f32[1],
		norm_vec.m128_f32[2] - mi_norm_vec.m128_f32[2]
	};

	// �񓪊p�x��K�p
	differ.m128_f32[0] /= rot_dead_zone_;
	differ.m128_f32[1] /= rot_dead_zone_;
	differ.m128_f32[2] /= rot_dead_zone_;

	// ���x�����Z
	mi_args_.vel.x += differ.m128_f32[0] * speed;
	mi_args_.vel.y += differ.m128_f32[1] * speed;
	mi_args_.vel.z = speed;

	// �ʒu���X�V
	pos.x += mi_args_.vel.x;
	pos.y += mi_args_.vel.y;
	pos.z += mi_args_.vel.z;

	// �ʒu�𔽉f
	obj_->SetPosition(pos);
}

void Missile::TermEmitter()
{
	emitter_->PrepareTerminate();
}

void Missile::UpdateEmitter()
{
	emitter_->SetPosition(obj_->GetPosition());
	emitter_->GenerateParticle();
}
