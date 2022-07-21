#include "QDSplashScene.h"
#include "SceneManager.h"

QDSplashScene::QDSplashScene() {

	Sprite::LoadTexture(static_cast<UINT>(TexNum::Alloc::LOGO), L"Resources/Textures/Logo/big_logo_alpha.png");
	logo_ = make_unique<Sprite>();
}

QDSplashScene::~QDSplashScene() {
}

void QDSplashScene::Initialize() {

	logo_ = static_cast<unique_ptr<Sprite>>(Sprite::Create(static_cast<UINT>(TexNum::Alloc::LOGO), { 0, 0 }));

	float center_x = logo_->GetTexSize().x / 2;
	float center_y = logo_->GetTexSize().y / 2;

	logo_->SetPosition({ Win32App::window_center_x_ - center_x,
					   Win32App::window_center_y_ - center_y });

	display_time_ = 100;
}

void QDSplashScene::Finalize() {
}

void QDSplashScene::Update() {

	static int count = 0;

	count++;

	if (display_time_ <= count) {

		Finalize();
		count = 0;
		SceneManager::SetScene(Scene::TITLE);
	}
}

void QDSplashScene::Draw() {

	Sprite::PreDraw();

	logo_->Draw();
}

void QDSplashScene::DebugDraw() {
}
