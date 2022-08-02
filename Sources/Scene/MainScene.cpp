#include "MainScene.h"
#include "../PreDraw/PreDraw.h"
#include "../Input/KeyboardInput.h"

MainScene::MainScene() {

	// カメラの生成
	camera_ = std::make_unique<Camera>();

	// プレイヤーの生成
	player_ = std::make_shared<Player>();

	texture_ = std::make_unique<Sprite>();
	Sprite::GenerateTexture(1, { 100, 100 }, 0xff0000ff);
	//Sprite::LoadTexture(1, L"Resources/Textures/Temp/orenge_texture.png");
	texture_ = (std::unique_ptr<Sprite>)(Sprite::Create(1, { 0, 0 }));

	grid_ = std::make_unique<GridRender>();
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

	GridRender::SetCamera(camera_.get());
	grid_->Initialize({ 0, 0, 0 });
}

void MainScene::Finalize() {
}

void MainScene::Update() {

	camera_->BasicCameraMoveTrack(0.5f);

	if (KeyboardInput::TriggerKey(DIK_SPACE)) {

		camera_->ResetCamera();
	}

	player_->Update();
	grid_->Update();
}

void MainScene::Draw() {


	PreDraw::PreRender(PipelineName::Line);
	grid_->Draw();

	player_->Draw();

	PreDraw::PreRender(PipelineName::Sprite);
	texture_->Draw();
}

void MainScene::DebugDraw() {
}
