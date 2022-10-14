#include "DemoScene.h"
#include "../../../Lib/Fbx/FbxLoader.h"

using namespace std;

DemoScene::DemoScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// FBXモデルデータの生成
	/*fbx_model_ = make_unique<FbxModel>();

	fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");*/
	fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	fbx_obj_ = new FbxObject3d();

}

DemoScene::~DemoScene() {
}

void DemoScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	FbxObject3d::SetCamera(camera_.get());

	// FBXオブジェクトデータの初期化
	fbx_obj_->Initialize();
	fbx_obj_->SetModel(fbx_model_);
	fbx_obj_->SetScale({ 1.0f, 1.0f, 1.0f });
	fbx_obj_->SetPosition({ 0, 0, 0 });
}

void DemoScene::Finalize() {
}

void DemoScene::Update() {

	// 回転処理
	float rotation_y = fbx_obj_->GetRotation().y;
	rotation_y += 1.0f;
	fbx_obj_->SetRotation({ 0, rotation_y, 0 });

	// 更新
	fbx_obj_->PlayAnimation(0, true);
	fbx_obj_->Update();
}

void DemoScene::Draw() {

	fbx_obj_->Draw(cmd_list_.Get());
}

void DemoScene::DebugDraw() {
}
