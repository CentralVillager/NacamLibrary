#include "FrictionDemoScene.h"

FrictionDemoScene::FrictionDemoScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// モデルデータの生成
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/SkyDome/", "SkyDome.obj", "SkyDome.mtl");

	// オブジェクトデータの生成
	object_ = make_unique<Object3d>();
}

FrictionDemoScene::~FrictionDemoScene() {
}

void FrictionDemoScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// オブジェクトデータの初期化
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale({ 0.01f, 0.01f, 0.01f });
	object_->SetPosition({ 0, 10.0f, 0 });
}

void FrictionDemoScene::Finalize() {
}

void FrictionDemoScene::Update() {

	// 更新
	object_->Update();
}

void FrictionDemoScene::Draw() {

	// 描画
	Object3d::PreDraw();
	object_->Draw();
}

void FrictionDemoScene::DebugDraw() {
}
