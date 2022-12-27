#include "RepulsionDemoScene.h"
#include "../../Collision/Collision.h"
#include "../../DebugUtill/NcmImGui.h"
#include "../../../Lib/Input/KeyboardInput.h"

using namespace DirectX;

RepulsionDemoScene::RepulsionDemoScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// モデルデータの生成
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");

	// オブジェクトデータの生成
	ball1_ = make_unique<Object3d>();
	ball2_ = make_unique<Object3d>();
}

RepulsionDemoScene::~RepulsionDemoScene() {
}

void RepulsionDemoScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// オブジェクトデータの初期化
	ball1_->Initialize();
	ball1_->SetModel(model_.get());
	ball1_->SetScale({ 1.0f, 1.0f, 1.0f });
	ball1_->SetPos({ -10.0f, 0, 0 });

	ball2_->Initialize();
	ball2_->SetModel(model_.get());
	ball2_->SetScale({ 2.0f, 2.0f, 2.0f });
	ball2_->SetPos({ 10.0f, 0, 0 });

	// float3 -> vector
	sphere1_.center = XMLoadFloat3(&ball1_->GetPos());
	sphere2_.center = XMLoadFloat3(&ball2_->GetPos());

	// 当たり判定の半径を設定
	sphere1_.radius = 1.0f;
	sphere2_.radius = 2.0f;

	velocity1_ = 0.1f;
	velocity2_ = -0.1f;

	mass1_ = 1.0f;
	mass2_ = 2.0f;

	petus1_ = velocity1_ * mass1_;
	petus2_ = velocity2_ * mass2_;

	e_ = 0.5f;
}

void RepulsionDemoScene::Finalize() {
}

void RepulsionDemoScene::Update() {

	// 当たり判定確認
	if (Collision::CheckSphere2Sphere(sphere1_, sphere2_)) {

		is_hit_ = true;

		petus1_ = ((velocity2_ * mass2_) + (velocity2_ * mass2_)) * e_;
		petus2_ = ((velocity1_ * mass1_) + (velocity1_ * mass1_)) * e_;

	} else {

		is_hit_ = false;
	}

	// 位置を更新
	XMFLOAT3 pos = ball1_->GetPos();
	pos.x += petus1_;
	ball1_->SetPos(pos);

	pos = ball2_->GetPos();
	pos.x += petus2_;
	ball2_->SetPos(pos);

	// 当たり判定を更新
	sphere1_.center = XMLoadFloat3(&ball1_->GetPos());
	sphere2_.center = XMLoadFloat3(&ball2_->GetPos());

	// リセット
	if (KeyboardInput::TriggerKey(DIK_SPACE)) {

		ball1_->SetPos({ -10.0f, 0, 0 });
		ball2_->SetPos({ 10.0f, 0, 0 });

		velocity1_ = 0.1f;
		velocity2_ = -0.1f;

		mass1_ = 1.0f;
		mass2_ = 2.0f;

		petus1_ = velocity1_ * mass1_;
		petus2_ = velocity2_ * mass2_;
	}

	// 更新
	ball1_->Update();
	ball2_->Update();
}

void RepulsionDemoScene::Draw() {

	// 描画
	Object3d::PreDraw();
	ball1_->Draw();
	ball2_->Draw();
}

void RepulsionDemoScene::DebugDraw() {

	ImGui::Text("ball1");
	ImGui::Text("pos.x = [%f]", sphere1_.center.m128_f32[0]);

	ImGui::Text("ball2");
	ImGui::Text("pos.x = [%f]", sphere2_.center.m128_f32[0]);

	ImGui::Checkbox("is_hit", &is_hit_);

	ImGui::Text("Retry : SPACE");

	ImGui::Dummy(ImVec2(10.0f, 10.0f));
	ImGui::DragFloat("e", &e_, 0.01f, 0.0f, 1.0f);
}
