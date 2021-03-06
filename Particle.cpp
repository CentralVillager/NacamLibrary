#include "Particle.h"
#include "ImGuiManager.h"

void Particle::Initialize(Model *model, ParticleMember &particle) {

	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model);

	particle_ = std::make_shared<ParticleMember>();
	particle_->position_ = particle.position_;
	particle_->velocity_ = particle.velocity_;
	particle_->accel_ = particle.accel_;
	particle_->s_scale_ = particle.s_scale_;
	particle_->e_scale_ = particle.e_scale_;
	particle_->life_ = particle.life_;
	particle_->is_dead_ = false;
}

void Particle::Finalize() {
}

void Particle::Update() {

	if (particle_->frame_ >= particle_->life_) {

		particle_->is_dead_ = true;
	}

	// 経過フレーム数をカウント
	particle_->frame_++;

	// 進行度を0〜1の範囲に換算
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

void Particle::Draw() {

	// 描画
	Object3d::PreDraw();
	object_->Draw();
}

void Particle::DebugDraw() {

	/*ImGui::Text("pos_x : %f", particle_->position_.x);
	ImGui::Text("pos_y : %f", particle_->position_.y);
	ImGui::Text("pos_z : %f", particle_->position_.z);*/
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs) {

	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}