#include "SplashScene.h"
#include "SceneManager.h"

SplashScene::SplashScene() {

	Sprite::LoadTexture(static_cast<UINT>(TexNum::Alloc::LOGO), L"Resources/Textures/Logo/big_logo_alpha.png");
	logo_ = make_unique<Sprite>();
}

SplashScene::~SplashScene() {
}

void SplashScene::Initialize() {

	logo_ = static_cast<unique_ptr<Sprite>>(Sprite::Create(static_cast<UINT>(TexNum::Alloc::LOGO), { 0, 0 }));

	float center_x = logo_->GetTexSize().x / 2;
	float center_y = logo_->GetTexSize().y / 2;

	logo_->SetPosition({ Window::window_center_x_ - center_x,
					   Window::window_center_y_ - center_y });

	display_time_ = 100;
}

void SplashScene::Finalize() {
}

void SplashScene::Update() {

	static int count = 0;

	count++;

	if (display_time_ <= count) {

		Finalize();
		count = 0;
		SceneManager::SetScene(Scene::TITLE);
	}
}

void SplashScene::Draw() {

	Sprite::PreDraw();

	logo_->Draw();
}

void SplashScene::DebugDraw() {
}
