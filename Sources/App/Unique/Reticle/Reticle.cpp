#include "Reticle.h"
#include "../Sources/Lib/Win32App/Win32App.h"

int Reticle::reticle_;

Reticle::Reticle()
{}

Reticle::~Reticle()
{}

void Reticle::LoadResources()
{
	//NcmSprite::LoadTexture((int)(TexNum::Reticle), L"Resources/Textures/reticle.png");

	reticle_ = NcmSprite::LoadTex("Resources/Textures/reticle.png");
}

void Reticle::Initialize()
{
	/*reticle_ = (std::unique_ptr<NcmSprite>)(NcmSprite::Create((int)(TexNum::Reticle),
		{ Win32App::CENTER_.x, Win32App::CENTER_.y }));
	reticle_->SetSize({ 100.0f, 100.0f });
	reticle_->SetAnchorPoint({ 0.5f, 0.5f });*/

	NcmSprite::SetPos(reticle_, Win32App::CENTER_);
	NcmSprite::SetSize(reticle_, { 100.0f, 100.0f });
	NcmSprite::SetAnchorPoint(reticle_, { 0.5f, 0.5f });
}

void Reticle::Update()
{}

void Reticle::Draw()
{
	//reticle_->Draw();
	NcmSprite::DrawTex(reticle_);
}
