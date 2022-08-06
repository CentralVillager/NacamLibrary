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
	missile_mgr_ = std::make_unique<MissileManager>();
}

MainScene::~MainScene() {
}

void MainScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, 0));
	camera_->MoveEye(XMFLOAT3(0, 10.0f, 0));
	Object3d::SetCamera(camera_.get());

	player_->Initialize();

	GridRender::SetCamera(camera_.get());
	grid_->Initialize(200, 10, XMFLOAT3(0, 0, 0));
	missile_mgr_->Initialize();
}

void MainScene::Finalize() {
}

void MainScene::Update() {

	camera_->BasicCameraMoveTrack(2.0f);

	if (KeyboardInput::TriggerKey(DIK_1)) {

		//camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, 0));
		camera_->SetEye(XMFLOAT3(0, 20.0f, -20.0f));
		camera_->SetTarget(XMFLOAT3(0, 10.0f, 0));
	}

	if (KeyboardInput::TriggerKey(DIK_SPACE)) {

		missile_mgr_->Fire();
	}

	player_->Update();
	grid_->Update();
	missile_mgr_->Update();
}

void MainScene::Draw() {

	PreDraw::PreRender(PipelineName::Line);
	grid_->Draw();

	PreDraw::PreRender(PipelineName::Object3d_WireFrame);
	player_->Draw();
	missile_mgr_->Draw();

	PreDraw::PreRender(PipelineName::Sprite);
	//texture_->Draw();
}

void MainScene::DebugDraw() {
}
