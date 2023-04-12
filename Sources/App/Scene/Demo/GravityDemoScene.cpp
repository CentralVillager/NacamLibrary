#include "GravityDemoScene.h"
#include "../../../Lib/DebugUtill/NcmImGui.h"

using namespace std;

GravityDemoScene::GravityDemoScene() {

	// �J�����̐���
	camera_ = make_unique<Camera>();

	// ���f���f�[�^�̐���
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/SkyDome/", "SkyDome.obj", "SkyDome.mtl");
	//model_->LoadObjModel("Resources/Monkey/", "Monkey.obj", "Monkey.mtl");

	// �I�u�W�F�N�g�f�[�^�̐���
	object_ = make_unique<Object3d>();
}

GravityDemoScene::~GravityDemoScene() {
}

void GravityDemoScene::Initialize() {

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// �I�u�W�F�N�g�f�[�^�̏�����
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale({ 0.01f, 0.01f, 0.01f });
	object_->SetPos({ 0, 10.0f, 0 });
}

void GravityDemoScene::Finalize() {
}

void GravityDemoScene::Update() {

	// ���݂̈ʒu���擾
	pos_ = object_->GetPos().y;

	// �ҋ@���邩
	if (!Wait()) {

		// ����
		Fall();
	}

	// �X�V
	object_->Update();
}

void GravityDemoScene::Draw() {

	// �`��
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

	// ���x��ω�
	velocity_ += g_;

	// �ʒu��ω�
	pos_ += -velocity_;

	// �ʒu���Z�b�g
	object_->SetPos({ 0, pos_, 0 });
}

bool GravityDemoScene::Wait() {

	// ���ɒ�������
	if (pos_ <= -10.0f) {

		// �ҋ@���Ԃ��J�E���g
		frame_count_++;

		// �ҋ@���Ԃ𖞗�������
		if (frame_count_ > 60) {

			// ���Ԃ����Z�b�g
			frame_count_ = 0;

			// �ʒu�Ƒ��x�����Z�b�g
			pos_ = 10.0f;
			object_->SetPos({ 0, pos_, 0 });
			velocity_ = 0;
		}

		return true;
	}

	return false;
}
