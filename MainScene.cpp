#include "MainScene.h"

MainScene::MainScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// 
	player_ = make_unique<Player>();
}

MainScene::~MainScene() {
}

void MainScene::Initialize() {
}

void MainScene::Finalize() {
}

void MainScene::Update() {
}

void MainScene::Draw() {
}

void MainScene::DebugDraw() {
}
