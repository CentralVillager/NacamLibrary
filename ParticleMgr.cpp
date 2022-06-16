#include "ParticleMgr.h"
#include "Utility.h"
#include "ImGuiManager.h"

using namespace DirectX;

void ParticleMgr::Initialize(int num) {

	// モデルデータの読み込みと生成
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");

	// リサイズ
	//particles_.resize(num);

	ParticleMember p{};
	p.scale_ = 1.0f;
	p.s_scale_ = 1.0f;
	p.e_scale_ = 0.0f;

	// 全てのパーティクルを初期化
	//for (auto &i : particles_) {

	//	/*p.position_ = NacamUtility::GenerateRandom({ -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f });
	//	p.velocity_ = NacamUtility::GenerateRandom({ -0.05f, -0.05f, -0.05f }, { 0.05f, 0.05f, 0.05f });*/

	//	/*p.position_.x = NacamUtility::GenerateRandom(-1, 1);
	//	p.position_.y = NacamUtility::GenerateRandom(-1, 1);
	//	p.position_.z = NacamUtility::GenerateRandom(-1, 1);
	//	p.velocity_.x = NacamUtility::GenerateRandom(-1, 1);
	//	p.velocity_.y = NacamUtility::GenerateRandom(-1, 1);
	//	p.velocity_.z = NacamUtility::GenerateRandom(-1, 1);*/

	//	const float rnd_pos = 10.0f;
	//	XMFLOAT3 pos{};
	//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	//	const float rnd_vel = 0.1f;
	//	XMFLOAT3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	//	XMFLOAT3 acc{};
	//	const float rnd_acc = 0.001f;
	//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//	p.position_ = pos;
	//	p.velocity_ = vel;
	//	p.accel_ = acc;

	//	i.Initialize(model_.get(), p, 100);
	//}

	p_.num_frame_ = 100;
	p_.s_scale_ = 1.0f;
	Add(100, GenerateValue(p_.accel_, p_.num_frame_));
}

void ParticleMgr::Finalize() {
}

void ParticleMgr::Update() {

	static int count = 0;
	count++;

	if (count >= interval_) {

		Add(p_.num_frame_, GenerateValue(p_.accel_, p_.num_frame_));
		count = 0;
	}

	// 寿命が尽きた粒をコンテナから削除
	particles_.remove_if([](Particle &x) {
		return x.GetIsDead(); }
	);

	for (auto &i : particles_) {

		i.Update();
	}
}

void ParticleMgr::Draw() {

	// 全てのパーティクルを描画
	for (auto &i : particles_) {

		i.Draw();
	}
}

void ParticleMgr::DebugDraw() {

	/*float vel[3] = {

		GenerateValue().velocity_.x,
		GenerateValue().velocity_.y,
		GenerateValue().velocity_.z,
	};
	ImGui::SliderFloat3("velocity", vel, 0.1f, 10.0f);
	p.velocity_.x = vel[0];
	p.velocity_.y = vel[1];
	p.velocity_.z = vel[2];*/

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
	ImGui::InputInt("life", &p_.num_frame_, 1, 10000);
	ImGui::SliderInt("GenerateIntervel", &interval_, 0, 100);

	for (auto &i : particles_) {

		i.DebugDraw();
	}
}

ParticleMember ParticleMgr::GenerateValue() {

	const float rnd_pos = 10.0f;
	XMFLOAT3 pos{};
	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	const float rnd_vel = 0.1f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	XMFLOAT3 acc{};
	const float rnd_acc = 0.001f;
	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	ParticleMember p{};
	p.position_ = pos;
	p.velocity_ = vel;
	p.accel_ = acc;
	p.scale_ = 1.0f;
	p.s_scale_ = 1.0f;
	p.e_scale_ = 0.0f;

	return p;
}

ParticleMember ParticleMgr::GenerateValue(XMFLOAT3 accel, int life) {

	const float rnd_pos = 10.0f;
	XMFLOAT3 pos{};
	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	const float rnd_vel = 0.1f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	ParticleMember p{};
	p.position_ = pos;
	p.velocity_ = vel;
	p.accel_ = p_.accel_;
	p.scale_ = p_.s_scale_;
	p.s_scale_ = p_.s_scale_;
	p.e_scale_ = 0.0f;
	p.num_frame_ = p_.num_frame_;

	return p;
}

void ParticleMgr::Add(int life, ParticleMember p) {

	particles_.emplace_front();
	Particle &p_ = particles_.front();
	p_.Initialize(model_.get(), p, life);
}
