#include "MainScene.h"

MainScene::MainScene() {

	// カメラの生成
	camera_ = std::make_unique<Camera>();

	// プレイヤーの生成
	player_ = std::make_shared<Player>();
}

MainScene::~MainScene() {
}

void MainScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 0, 0 });
	Object3d::SetCamera(camera_.get());

	player_->Initialize();
}

void MainScene::Finalize() {
}

void MainScene::Update() {

	camera_->Update();

	player_->Update();
}

void MainScene::Draw() {

	player_->Draw();
}

void MainScene::DebugDraw() {
}
