#include "Particle.h"
#include "../Debug/NcmImGui.h"

Particle::Particle()
{
	object_ = std::make_unique<Object3d>();
	particle_ = std::make_shared<ParticleMember>();
}

Particle::~Particle()
{}

void Particle::Initialize(Model *model, const ParticleMember &particle)
{
	object_->SetModel(model);
	object_->Initialize();

	particle_->position_ = particle.position_;
	particle_->velocity_ = particle.velocity_;
	particle_->accel_ = particle.accel_;
	particle_->s_scale_ = particle.s_scale_;
	particle_->e_scale_ = particle.e_scale_;
	particle_->life_ = particle.life_;
	particle_->is_dead_ = false;
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
	object_->SetPosition(particle_->position_);
	object_->SetScale(particle_->scale_);
	object_->Update();
}

void Particle::Draw()
{
	// �`��
	object_->Draw();
}

void Particle::DebugDraw()
{}

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
