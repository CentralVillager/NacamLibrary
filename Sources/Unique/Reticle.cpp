#include "Reticle.h"
#include "../Win32App/Win32App.h"

int Reticle::reticle_;

Reticle::Reticle()
{}

Reticle::~Reticle()
{}

void Reticle::LoadResources()
{
	//Sprite::LoadTexture((int)(TexNum::Reticle), L"Resources/Textures/reticle.png");
	reticle_ = Sprite::LoadTex(L"Resources/Textures/reticle.png");
}

void Reticle::Initialize()
{
	/*reticle_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Reticle),
		{ Win32App::CENTER_.x, Win32App::CENTER_.y }));
	reticle_->SetSize({ 100.0f, 100.0f });
	reticle_->SetAnchorPoint({ 0.5f, 0.5f });*/

	Sprite::SetPos(reticle_, Win32App::CENTER_);
	Sprite::SetSize(reticle_, { 100.0f, 100.0f });
	Sprite::SetAnchorPoint(reticle_, { 0.5f, 0.5f });
}

void Reticle::Update()
{}

void Reticle::Draw()
{
	//reticle_->Draw();
	Sprite::DrawTex(reticle_);
}
