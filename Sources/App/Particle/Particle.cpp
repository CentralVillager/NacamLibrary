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
	// 寿命を迎えたら
	if (particle_->frame_ >= particle_->life_)
	{
		// 死亡フラグを立てる
		particle_->is_dead_ = true;
	}

	// 経過フレーム数をカウント
	particle_->frame_++;

	// 進行度を0～1の範囲に換算
	float f = (float)particle_->life_ / particle_->frame_;

	// 速度に加速度を加算
	particle_->velocity_ = particle_->velocity_ + particle_->accel_;

	// 速度による移動
	particle_->position_ = particle_->position_ + particle_->velocity_;

	// スケールの線形補間
	particle_->scale_ = particle_->s_scale_ + (particle_->e_scale_ - particle_->s_scale_) / f;

	//	最終的な位置をセット
	object_->SetPosition(particle_->position_);
	object_->SetScale(particle_->scale_);
	object_->Update();
}

void Particle::Draw()
{
	// 描画
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
