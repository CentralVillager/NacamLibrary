#include "ParticleDemoScene.h"
#include "KeyboardInput.h"
#include "ImGuiManager.h"

ParticleDemoScene::ParticleDemoScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// パーティクルデータの生成
	contrail_1_ = make_unique<Emitter>();
	contrail_2_ = make_unique<Emitter>();
	emitter_1_ = make_unique<Emitter>();
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
	contrail_1_->particle_args_.member_.position_ = { 10.0f, 0.0f, -50.0f };
	contrail_1_->particle_args_.member_.velocity_ = { 0.0f, 0.f, -1.0f };
	contrail_1_->particle_args_.member_.accel_ = { 0, 0.001f, 0 };
	contrail_1_->particle_args_.member_.life_ = 100;
	contrail_1_->particle_args_.member_.s_scale_ = 1.0f;
	contrail_1_->particle_args_.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	contrail_1_->particle_args_.gene_num_ = 1;

	contrail_2_->particle_args_.member_.position_ = { -10.0f, 0.0f, -50.0f };
	contrail_2_->particle_args_.member_.velocity_ = { 0.0f, 0.f, -1.0f };
	contrail_2_->particle_args_.member_.accel_ = { 0, 0.001f, 0 };
	contrail_2_->particle_args_.member_.life_ = 100;
	contrail_2_->particle_args_.member_.s_scale_ = 1.0f;
	contrail_2_->particle_args_.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	contrail_2_->particle_args_.gene_num_ = 1;
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

	if (KeyboardInput::PushKey(DIK_SPACE)) {

		contrail_1_->particle_args_.member_.position_ = { 10.0f, 0.0f, -50.0f };
		contrail_2_->particle_args_.member_.position_ = { -10.0f, 0.0f, -50.0f };
	}

	// パーティクルを生成
	contrail_1_->particle_args_.member_.position_.z += 1.0f;
	contrail_2_->particle_args_.member_.position_.z += 1.0f;

	contrail_1_->GenerateParticle();
	contrail_2_->GenerateParticle();
}

void ParticleDemoScene::Draw() {

	// 描画
	contrail_1_->Draw();
	contrail_2_->Draw();
}

void ParticleDemoScene::DebugDraw() {

	/*Emitter::ParticleArgs *ptr = &contrail_1_->particle_args_;
	ImGuiManager::SliderFloat3Helper("pos", ptr->member_.position_, -10.0f, 10.0f);
	ImGuiManager::SliderFloat3Helper("range", ptr->pos_rand_, -5.0f, 5.0f);
	ImGuiManager::SliderFloat3Helper("velocity", ptr->member_.velocity_, -5.0f, 5.0f);
	ImGuiManager::SliderFloat3Helper("accel", ptr->member_.accel_, -5.0f, 5.0f);
	ImGui::SliderFloat("scale", &ptr->member_.scale_, 0.1f, 10.0f);
	ImGui::InputInt("life", &ptr->member_.life_, 1, 10000);
	ImGuiManager::SliderUINTHelper("num", ptr->gene_num_, 0, 50);*/

	contrail_1_->DebugDraw("1");
	contrail_2_->DebugDraw("2");
}
