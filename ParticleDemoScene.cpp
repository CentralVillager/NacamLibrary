#include "ParticleDemoScene.h"
#include "KeyboardInput.h"

ParticleDemoScene::ParticleDemoScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// パーティクルデータの生成
	test_particle_ = make_unique<ParticleMgr>();
}

ParticleDemoScene::~ParticleDemoScene() {
}

void ParticleDemoScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// パーティクルの初期化
	test_particle_->Initialize(100);
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

	// 更新
	test_particle_->Update();
}

void ParticleDemoScene::Draw() {

	// 描画
	test_particle_->Draw();
}

void ParticleDemoScene::DebugDraw() {

	test_particle_->DebugDraw();
}
