#include "DemoScene.h"
#include "../../../Lib/Fbx/FbxLoader.h"

using namespace std;

DemoScene::DemoScene() {

	// �J�����̐���
	camera_ = make_unique<Camera>();

	// FBX���f���f�[�^�̐���
	/*fbx_model_ = make_unique<FbxModel>();

	fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");*/
	fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	fbx_obj_ = new FbxObject3d();

}

DemoScene::~DemoScene() {
}

void DemoScene::Initialize() {

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	FbxObject3d::SetCamera(camera_.get());

	// FBX�I�u�W�F�N�g�f�[�^�̏�����
	fbx_obj_->Initialize();
	fbx_obj_->SetModel(fbx_model_);
	fbx_obj_->SetScale({ 1.0f, 1.0f, 1.0f });
	fbx_obj_->SetPosition({ 0, 0, 0 });
}

void DemoScene::Finalize() {
}

void DemoScene::Update() {

	// ��]����
	float rotation_y = fbx_obj_->GetRotation().y;
	rotation_y += 1.0f;
	fbx_obj_->SetRotation({ 0, rotation_y, 0 });

	// �X�V
	fbx_obj_->PlayAnimation(0, true);
	fbx_obj_->Update();
}

void DemoScene::Draw() {

	fbx_obj_->Draw(cmd_list_.Get());
}

void DemoScene::DebugDraw() {
}
