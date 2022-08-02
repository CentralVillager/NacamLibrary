#include "FrictionDemoScene.h"

FrictionDemoScene::FrictionDemoScene() {

	// �J�����̐���
	camera_ = make_unique<Camera>();

	// ���f���f�[�^�̐���
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/SkyDome/", "SkyDome.obj", "SkyDome.mtl");

	// �I�u�W�F�N�g�f�[�^�̐���
	object_ = make_unique<Object3d>();
}

FrictionDemoScene::~FrictionDemoScene() {
}

void FrictionDemoScene::Initialize() {

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// �I�u�W�F�N�g�f�[�^�̏�����
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale({ 0.01f, 0.01f, 0.01f });
	object_->SetPosition({ 0, 10.0f, 0 });
}

void FrictionDemoScene::Finalize() {
}

void FrictionDemoScene::Update() {

	// �X�V
	object_->Update();
}

void FrictionDemoScene::Draw() {

	// �`��
	Object3d::PreDraw();
	object_->Draw();
}

void FrictionDemoScene::DebugDraw() {
}
