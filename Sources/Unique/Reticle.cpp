#include "Reticle.h"
#include "../Win32App/Win32App.h"

Reticle::Reticle()
{
	reticle_ = make_unique<Sprite>();
}

Reticle::~Reticle()
{}

void Reticle::LoadResources()
{
	Sprite::LoadTexture((int)(TexNum::Reticle), L"Resources/Textures/reticle.png");
}

void Reticle::Initialize()
{
	reticle_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Reticle),
		{ Win32App::window_center_x_, Win32App::window_center_y_ }));
	reticle_->SetSize({ 100.0f, 100.0f });
	reticle_->SetAnchorPoint({ 0.5f, 0.5f });
}

void Reticle::Update()
{}

void Reticle::Draw()
{
	reticle_->Draw();
}
