#include "QDTitleScene.h"
#include "Win32App.h"
#include "SceneManager.h"
#include "BackBoardManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"

QDTitleScene::QDTitleScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// サウンドの初期化
	bgm_ = make_unique<AudioManager>();
	bgm_->LoadWaveFile("Resources/Sounds/", "main");

	// UIの読み込み
	Sprite::LoadTexture(static_cast<UINT>(TexNum::Alloc::QUANTUM), L"Resources/Textures/QuantumDrive_ReRe.png");

	// 描画データの生成
	quantum_drive_ = make_unique<Sprite>();
}

QDTitleScene::~QDTitleScene() {
}

void QDTitleScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	Object3d::SetCamera(camera_.get());
	FbxObject3d::SetCamera(camera_.get());

	// 描画データの代入
	quantum_drive_ = static_cast<unique_ptr<Sprite>>(Sprite::Create(static_cast<UINT>(TexNum::Alloc::QUANTUM), { 0, 0 }));

	// 中心を取る
	float center_x = quantum_drive_->GetTexSize().x / 2;
	float center_y = quantum_drive_->GetTexSize().y / 2;

	// 画面の中心に配置
	quantum_drive_->SetPosition({ Win32App::window_center_x_ - center_x,
					   Win32App::window_center_y_ - center_y });

	// 音声の再生
	//main_->PlayWave(0.1f, AudioManager::LOOP, false);

	// ボードの初期化
	BackBoardManager::GetInstance()->Initialize();
}

void QDTitleScene::Finalize() {

	// BGMを止める
	bgm_->StopWave();

	// アンロードする
	bgm_->UnloadWaveFile();
}

void QDTitleScene::Update() {

	// ボードの更新
	BackBoardManager::GetInstance()->Update();

	// ・スペースを押す
	// ・パッドAを押す
	if (KeyboardInput::TriggerKey(DIK_SPACE) || ControllerInput::TriggerButton(DIB_A)) {

		// 終了処理
		Finalize();

		// メインへ
		SceneManager::SetScene(Scene::MAIN);
	}
}

void QDTitleScene::Draw() {

	Object3d::PreDraw();

	// ボードの描画
	BackBoardManager::GetInstance()->Draw();

	Sprite::PreDraw();

	// UIの描画
	quantum_drive_->Draw();
}

void QDTitleScene::DebugDraw() {
}
