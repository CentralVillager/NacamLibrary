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

void Emitter::DebugDraw(const string &name) {

	ImGuiManager::SliderFloat3Helper("pos", particle_args_.member_.position_, -10.0f, 10.0f);
	ImGuiManager::SliderFloat3Helper("range", particle_args_.pos_rand_, -5.0f, 5.0f);
	ImGuiManager::SliderFloat3Helper("velocity", particle_args_.member_.velocity_, -5.0f, 5.0f);
	ImGuiManager::SliderFloat3Helper("accel", particle_args_.member_.accel_, -5.0f, 5.0f);
	ImGui::SliderFloat("scale", &particle_args_.member_.scale_, 0.1f, 10.0f);
	ImGui::InputInt("life", &particle_args_.member_.life_, 1, 10000);
	ImGuiManager::SliderUINTHelper("num", particle_args_.gene_num_, 0, 50);
	ImGui::Text("");
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
	/*for (num_per_frame; num_per_frame > 0; num_per_frame--) {

		Add(GenerateValue(p, rand_range));
	}*/

	// 1フレーム中に指定された数を生成する
	for (UINT count = particle_args_.gene_num_; count > 0; count--) {

		Add(GenerateValue(particle_args_.member_, particle_args_.pos_rand_));
	}

	// 寿命が尽きた粒をコンテナから削除
	particles_.remove_if([](Particle &x) {
		return x.GetIsDead(); }
	);

	// 全てのパーティクルを更新
	for (auto &i : particles_) {

		i.Update();
	}
}

void Emitter::GenerateParticle() {

	// 1フレーム中に指定された数を生成する
	for (UINT count = particle_args_.gene_num_; count > 0; count--) {

		Add(GenerateValue(particle_args_.member_, particle_args_.pos_rand_));
	}

	// 寿命が尽きた粒をコンテナから削除
	particles_.remove_if([](Particle &x) {
		return x.GetIsDead(); }
	);

	// 全てのパーティクルを更新
	for (auto &i : particles_) {

		i.Update();
	}
}
