#include "PostEffectDemoScene.h"
#include "../../DrawProc/DrawProc.h"
#include "../../Input/KeyboardInput.h"
#include "../../Debug/ImGuiManager.h"

PostEffectDemoScene::PostEffectDemoScene()
{
	// �J�����̐���
	camera_ = make_unique<Camera>();

	// ���f���f�[�^�̐���
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Enemy/", "Enemy.obj", "Enemy.mtl");

	// �I�u�W�F�N�g�f�[�^�̐���
	object_ = make_unique<Object3d>();

	// �w�i�̐���
	back_ = Sprite::LoadTex(L"Resources/Textures/background.png");
}

PostEffectDemoScene::~PostEffectDemoScene()
{}

void PostEffectDemoScene::Initialize()
{
	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());

	// �I�u�W�F�N�g�f�[�^�̏�����
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale(1.0f);
	object_->SetPosition({ 0, 0, 0 });

	is_push_ = false;
}

void PostEffectDemoScene::Finalize()
{}

void PostEffectDemoScene::Update()
{
	// Y����]
	RollY();

	// �X�V
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
	/*Sprite::PreDraw();
	back_->Draw();
	DrawProc::ClearDepthBuffer();*/

	// �`��
	Object3d::PreDraw();
	object_->Draw();
}

void PostEffectDemoScene::DebugDraw()
{
	ImGui::Checkbox("Pushed", &is_push_);
}

void PostEffectDemoScene::RollY()
{
	float rotation = object_->GetRotation().y;

	rotation += 1.0f;

	object_->SetRotation({ 0, rotation, 0 });
}
