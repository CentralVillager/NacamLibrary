#include "Particle.h"
#include "../Debug/NcmImGui.h"
#include "../../Lib/PreDraw/PreDraw.h"
#include "../../Lib/Sprite/NcmSprite.h"

Particle::Particle() :
	object_(std::make_unique<Object3d>()),
	particle_(std::make_shared<ParticleDesc>()),
	plate_(std::make_unique<PlatePoly>())
{}

Particle::~Particle()
{}

void Particle::Initialize(Model *model, const ParticleDesc &particle)
{
	particle_->position_ = particle.position_;
	particle_->velocity_ = particle.velocity_;
	particle_->accel_ = particle.accel_;
	particle_->s_scale_ = particle.s_scale_;
	particle_->e_scale_ = particle.e_scale_;
	particle_->life_ = particle.life_;
	particle_->is_dead_ = false;
	particle_->tex_handle_ = particle.tex_handle_;

	object_->SetModel(model);
	object_->Initialize();
	plate_->Initialize(particle_->tex_handle_);
	plate_->SetTexHandle(particle_->tex_handle_);
}

void Particle::Finalize()
{}

void Particle::Update()
{
	// �������}������
	if (particle_->frame_ >= particle_->life_)
	{
		// ���S�t���O�𗧂Ă�
		particle_->is_dead_ = true;
	}

	// �o�߃t���[�������J�E���g
	particle_->frame_++;

	// �i�s�x��0�`1�͈̔͂Ɋ��Z
	float f = (float)particle_->life_ / particle_->frame_;

	// ���x�ɉ����x�����Z
	particle_->velocity_ = particle_->velocity_ + particle_->accel_;

	// ���x�ɂ��ړ�
	particle_->position_ = particle_->position_ + particle_->velocity_;

	// �X�P�[���̐��`���
	particle_->scale_ = particle_->s_scale_ + (particle_->e_scale_ - particle_->s_scale_) / f;

	//	�ŏI�I�Ȉʒu���Z�b�g
	/*object_->SetPos(particle_->position_);
	object_->SetScale(particle_->scale_);
	object_->Update();*/
	plate_->SetPos(particle_->position_);
	plate_->SetScale(particle_->scale_);
	plate_->Update();
}

void Particle::Draw()
{
	//PreDraw::PreRender(PipelineName::Object3d_WireFrame);	// ��
	//// �`��
	//object_->Draw();

	PreDraw::PreRender(PipelineName::PlatePoly);
	plate_->Draw();
}

void Particle::DebugDraw()
{}

void Particle::ResetParamater()
{
	// �[���N���A
	ZeroMemory(particle_.get(), sizeof(ParticleDesc));
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3 &lhs, const int &rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	return result;
}
