#include "Emitter.h"
#include "Utility.h"
#include "ImGuiManager.h"

using namespace DirectX;

std::unique_ptr<Model> Emitter::model_ = nullptr;

void Emitter::StaticInitialize() {

	// モデルデータの読み込みと生成
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
}

void Emitter::Draw() {

	// 全てのパーティクルを描画
	for (auto &i : particles_) {

		i.Draw();
	}
}

void Emitter::DebugDraw() {

	/*float pos[3] = {

		p_.position_.x,
		p_.position_.y,
		p_.position_.z,
	};
	ImGui::SliderFloat3("pos", pos, -5.0f, 5.0f);
	p_.position_.x = pos[0];
	p_.position_.y = pos[1];
	p_.position_.z = pos[2];

	float range[3] = {

		range_.x,
		range_.y,
		range_.z,
	};
	ImGui::SliderFloat3("range", range, -5.0f, 5.0f);
	range_.x = range[0];
	range_.y = range[1];
	range_.z = range[2];

	float vel[3] = {

		p_.velocity_.x,
		p_.velocity_.y,
		p_.velocity_.z,
	};
	ImGui::SliderFloat3("velocity", vel, -1.0f, 1.0f);
	p_.velocity_.x = vel[0];
	p_.velocity_.y = vel[1];
	p_.velocity_.z = vel[2];

	float acc[3] = {

		p_.accel_.x,
		p_.accel_.y,
		p_.accel_.z,
	};
	ImGui::SliderFloat3("accel", acc, -1.0f, 1.0f);
	p_.accel_.x = acc[0];
	p_.accel_.y = acc[1];
	p_.accel_.z = acc[2];

	ImGui::SliderFloat("scale", &p_.s_scale_, 0.1f, 10.0f);
	ImGui::InputInt("life", &p_.life_, 1, 10000);
	ImGui::SliderInt("GenerateIntervel", &interval_, 0, 100);*/

	for (auto &i : particles_) {

		i.DebugDraw();
	}
}

ParticleMember Emitter::GenerateValue(ParticleMember p, XMFLOAT3 rand_range) {

	using namespace NacamUtility;

	XMFLOAT3 pos = GenerateRandom({ p.position_.x - rand_range.x, p.position_.y - rand_range.y, p.position_.z - rand_range.z }, { p.position_.x + rand_range.x, p.position_.y + rand_range.y, p.position_.z + rand_range.z });

	float range = 0.1f;
	XMFLOAT3 vel = GenerateRandom({ p.velocity_.x - range, p.velocity_.y - range, p.velocity_.z - range }, { p.velocity_.x + range, p.velocity_.y + range, p.velocity_.z + range });

	XMFLOAT3 acc{};
	acc.y = 0.001f;

	ParticleMember _p{};
	_p.position_ = pos;
	_p.velocity_ = vel;
	_p.accel_ = acc;
	_p.scale_ = p.s_scale_;
	_p.s_scale_ = p.s_scale_;
	_p.e_scale_ = 0.0f;
	_p.life_ = p.life_;

	return _p;
}

void Emitter::Add(ParticleMember p) {

	// 先頭に要素を構築
	particles_.emplace_front();

	// 先頭の参照を取得
	Particle &p_ = particles_.front();

	// 初期化
	p_.Initialize(model_.get(), p);
}

void Emitter::GenerateParticle(ParticleMember p, XMFLOAT3 rand_range, int num_per_frame) {

	// 1フレーム中に指定された数を生成する
	for (num_per_frame; num_per_frame > 0; num_per_frame--) {

		Add(GenerateValue(p, rand_range));
	}

	// 寿命が尽きた粒をコンテナから削除
	particles_.remove_if([](Particle &x) {
		return x.GetIsDead(); }
	);

	for (auto &i : particles_) {

		i.Update();
	}
}
