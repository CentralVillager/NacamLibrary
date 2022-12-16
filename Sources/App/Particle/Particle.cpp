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
	// 渡された各値を適用
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
	// 寿命を迎えたら
	if (particle_->frame_ >= particle_->life_)
	{
		// 死亡フラグを立てる
		particle_->is_dead_ = true;
	}

	// 経過フレーム数をカウント
	particle_->frame_++;

	// 進行度を0~1の範囲に換算
	float f = (float)(particle_->life_) / particle_->frame_;

	// 速度に加速度を加算
	particle_->velocity_ = particle_->velocity_ + particle_->accel_;

	// 速度による移動
	particle_->position_ = particle_->position_ + particle_->velocity_;

	// スケールの線形補間
	particle_->scale_ = particle_->s_scale_ + (particle_->e_scale_ - particle_->s_scale_) / f;

	// 透明度を0~1換算
	particle_->alpha_ = 1 - (float)(particle_->frame_) / particle_->life_;
}

void Particle::Draw()
{}

void Particle::DebugDraw()
{}

void Particle::ResetParamater()
{
	// ゼロクリア
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
