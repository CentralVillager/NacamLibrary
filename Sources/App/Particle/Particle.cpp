#include "Particle.h"
#include "../Debug/NcmImGui.h"
#include "../../Lib/PreDraw/PreDraw.h"
#include "../../Lib/Sprite/NcmSprite.h"

Particle::Particle() :
	particle_(std::make_shared<ParticleDesc>())
{}

Particle::~Particle()
{}

void Particle::Initialize(Model *model, const ParticleDesc &part_desc_)
{
	// �n���ꂽ�e�l��K�p
	particle_->position_ = part_desc_.position_;
	particle_->velocity_ = part_desc_.velocity_;
	particle_->accel_ = part_desc_.accel_;
	particle_->life_ = part_desc_.life_;
	particle_->s_scale_ = part_desc_.s_scale_;
	particle_->e_scale_ = part_desc_.e_scale_;
	particle_->is_dead_ = false;
	particle_->alpha_ = part_desc_.alpha_;
	particle_->tex_handle_ = part_desc_.tex_handle_;
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

	// �i�s�x��0~1�͈̔͂Ɋ��Z
	float f = (float)(particle_->life_) / particle_->frame_;

	// ���x�ɉ����x�����Z
	particle_->velocity_ = particle_->velocity_ + particle_->accel_;

	// ���x�ɂ��ړ�
	particle_->position_ = particle_->position_ + particle_->velocity_;

	// �X�P�[���̐��`���
	particle_->scale_ = particle_->s_scale_ + (particle_->e_scale_ - particle_->s_scale_) / f;

	// �����x��0~1���Z
	particle_->alpha_ = 1 - (float)(particle_->frame_) / particle_->life_;
}

void Particle::Draw()
{}

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
