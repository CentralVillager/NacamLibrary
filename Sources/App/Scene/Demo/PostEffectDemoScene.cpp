#include "PostEffectDemoScene.h"
#include "../../../Lib/DirectX/DrawProc.h"
#include "../../../Lib/Input/KeyboardInput.h"
#include "../../../Lib/DebugUtill/NcmImGui.h"

PostEffectDemoScene::PostEffectDemoScene()
{
	// カメラの生成
	camera_ = make_unique<Camera>();

	// モデルデータの生成
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Enemy/", "Enemy.obj", "Enemy.mt");

	// オブジェクトデータの生成
	object_ = make_unique<Object3d>();

	// 背景の生成
	back_ = NcmSprite::LoadTex("Resources/Textures/background.png");
}

PostEffectDemoScene::~PostEffectDemoScene()
{}

void PostEffectDemoScene::Initialize()
{
	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// オブジェクトデータの初期化
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale(1.0f);
	object_->SetPos({ 0, 0, 0 });

	is_push_ = false;
}

void PostEffectDemoScene::Finalize()
{}

void PostEffectDemoScene::Update()
{
	// Y軸回転
	RollY();

	// 更新
	object_->Update();

	if (KeyboardInput::PushKey(DIK_SPACE) || KeyboardInput::PushKey(DIK_V))
	{
		is_push_ = true;
	}
	else
	{
		is_push_ = false;
	}
}

void PostEffectDemoScene::Draw()
{
	/*NcmSprite::PreDraw();
	back_->Draw();
	DrawProc::ClearDepthBuffer();*/

	// 描画
	Object3d::PreDraw();
	object_->Draw();
}

void PostEffectDemoScene::DebugDraw()
{
	ImGui::Checkbox("Pushed", &is_push_);
}

void PostEffectDemoScene::RollY()
{
	float rotation = object_->GetRot().y;

	rotation += 1.0f;

	object_->SetRot({ 0, rotation, 0 });
}
