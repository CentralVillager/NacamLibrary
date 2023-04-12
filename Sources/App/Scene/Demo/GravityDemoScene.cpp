#include "GravityDemoScene.h"
#include "../../../Lib/DebugUtill/NcmImGui.h"

using namespace std;

GravityDemoScene::GravityDemoScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// モデルデータの生成
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/SkyDome/", "SkyDome.obj", "SkyDome.mtl");
	//model_->LoadObjModel("Resources/Monkey/", "Monkey.obj", "Monkey.mtl");

	// オブジェクトデータの生成
	object_ = make_unique<Object3d>();
}

GravityDemoScene::~GravityDemoScene() {
}

void GravityDemoScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// オブジェクトデータの初期化
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale({ 0.01f, 0.01f, 0.01f });
	object_->SetPos({ 0, 10.0f, 0 });
}

void GravityDemoScene::Finalize() {
}

void GravityDemoScene::Update() {

	// 現在の位置を取得
	pos_ = object_->GetPos().y;

	// 待機するか
	if (!Wait()) {

		// 落下
		Fall();
	}

	// 更新
	object_->Update();
}

void GravityDemoScene::Draw() {

	// 描画
	Object3d::PreDraw();
	object_->Draw();
}

void GravityDemoScene::AfterPostEffectDraw()
{}

void GravityDemoScene::DebugDraw() {

	ImGui::Text("pos.y = %f", object_->GetPos().y);
	ImGui::Text("G = %f", g_);
	ImGui::Text("velocity = %f", velocity_);
	ImGui::Text("frame_count = %d", frame_count_);

	ImGui::InputFloat("acceleration", &g_, 0.0f, 1.0f);
	ImGui::SliderFloat("acceleration_", &g_, 0.0f, 1.0f);
	ImGui::Text("ResetValue = 0.163");
}

void GravityDemoScene::Fall() {

	// 速度を変化
	velocity_ += g_;

	// 位置を変化
	pos_ += -velocity_;

	// 位置をセット
	object_->SetPos({ 0, pos_, 0 });
}

bool GravityDemoScene::Wait() {

	// 下に着いたら
	if (pos_ <= -10.0f) {

		// 待機時間をカウント
		frame_count_++;

		// 待機時間を満了したら
		if (frame_count_ > 60) {

			// 時間をリセット
			frame_count_ = 0;

			// 位置と速度をリセット
			pos_ = 10.0f;
			object_->SetPos({ 0, pos_, 0 });
			velocity_ = 0;
		}

		return true;
	}

	return false;
}
