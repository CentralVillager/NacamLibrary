#include "QDResultScene.h"
#include "SceneManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "BackBoardManager.h"

QDResultScene::QDResultScene() {

	// ゲームオーバーUIのロード
	Sprite::LoadTexture(static_cast<UINT>(TexNum::Alloc::RESULT), L"Resources/Textures/game_is_over.png");

	// 描画データの生成
	game_over_ = make_unique<Sprite>();
}

QDResultScene::~QDResultScene() {
}

void QDResultScene::Initialize() {

	// スプライトデータを代入
	game_over_ = static_cast<unique_ptr<Sprite>>(Sprite::Create(static_cast<UINT>(TexNum::Alloc::RESULT), { 0, 0 }));

	// スプライトの中央を取得
	float center_x = game_over_->GetTexSize().x / 2;
	float center_y = game_over_->GetTexSize().y / 2;

	// 画面の中央に配置
	game_over_->SetPosition({ Win32App::window_center_x_ - center_x,
					   Win32App::window_center_y_ - center_y });

	// ボードの初期化
	BackBoardManager::GetInstance()->Initialize();
}

void QDResultScene::Finalize() {
}

void QDResultScene::Update() {

	// ボードの更新
	BackBoardManager::GetInstance()->Update();

	// ・スペースを押す
	// ・パッドAを押す
	if (KeyboardInput::TriggerKey(DIK_SPACE) || ControllerInput::TriggerButton(DIB_A)) {

		// タイトルへ
		SceneManager::SetScene(Scene::TITLE);
	}
}

void QDResultScene::Draw() {

	Object3d::PreDraw();

	// ボードの描画
	BackBoardManager::GetInstance()->Draw();

	Sprite::PreDraw();

	// UIの描画
	game_over_->Draw();
}

void QDResultScene::DebugDraw() {
}
