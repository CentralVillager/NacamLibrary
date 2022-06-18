#include "ParticleDemoScene.h"
#include "KeyboardInput.h"
#include "ImGuiManager.h"

ParticleDemoScene::ParticleDemoScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// パーティクルデータの生成
	emitter_1_ = make_unique<Emitter>();
	emitter_2_ = make_unique<Emitter>();
}

ParticleDemoScene::~ParticleDemoScene() {
}

void ParticleDemoScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 0, 0 });
	Object3d::SetCamera(camera_.get());

	Emitter::StaticInitialize();

	// パーティクルの初期化
	p_.position_ = { 10.0f, 0.0f, -50.0f };
	p_.velocity_ = { 0.0f, 0.f, -1.0f };
	p_.accel_ = { 0, 0.001f, 0 };
	p_.life_ = 100;
	p_.s_scale_ = 1.0f;
	range_ = { 0.0f, 0.0f, 0.0f };
	generate_num_ = 1;

	p_2_.position_ = { -10.0f, 0.0f, -50.0f };
	p_2_.velocity_ = { 0.0f, 0.f, -1.0f };
	p_2_.accel_ = { 0, 0.001f, 0 };
	p_2_.life_ = 100;
	p_2_.s_scale_ = 1.0f;
}

void ParticleDemoScene::Finalize() {
}

void ParticleDemoScene::Update() {

	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F)) {

		if (KeyboardInput::PushKey(DIK_W)) { camera_->MoveCameraTrack({ 0.0f,+1.0f,0.0f }); } else if (KeyboardInput::PushKey(DIK_S)) { camera_->MoveCameraTrack({ 0.0f,-1.0f,0.0f }); }

		if (KeyboardInput::PushKey(DIK_D)) { camera_->MoveCameraTrack({ +1.0f, 0.0f, 0.0f }); } else if (KeyboardInput::PushKey(DIK_A)) { camera_->MoveCameraTrack({ -1.0f,0.0f,0.0f }); }

		if (KeyboardInput::PushKey(DIK_R)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, +1.0f }); } else if (KeyboardInput::PushKey(DIK_F)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, -1.0f }); }
	}

	camera_->Update();

	// パーティクルを生成
	p_.position_.z += 1.0f;
	p_2_.position_.z += 1.0f;
	emitter_1_->GenerateParticle(p_, range_, generate_num_);
	emitter_2_->GenerateParticle(p_2_, range_, generate_num_);
}

void ParticleDemoScene::Draw() {

	// 描画
	emitter_1_->Draw();
	emitter_2_->Draw();
}

void ParticleDemoScene::DebugDraw() {

	float pos[3] = {

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
	ImGui::SliderInt("num", &generate_num_, 0, 50);

	emitter_1_->DebugDraw();
}
