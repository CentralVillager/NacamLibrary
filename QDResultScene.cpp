#include "QDResultScene.h"
#include "SceneManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "BackBoardManager.h"

QDResultScene::QDResultScene() {

	// �Q�[���I�[�o�[UI�̃��[�h
	Sprite::LoadTexture(static_cast<UINT>(TexNum::Alloc::RESULT), L"Resources/Textures/game_is_over.png");

	// �`��f�[�^�̐���
	game_over_ = make_unique<Sprite>();
}

QDResultScene::~QDResultScene() {
}

void QDResultScene::Initialize() {

	// �X�v���C�g�f�[�^����
	game_over_ = static_cast<unique_ptr<Sprite>>(Sprite::Create(static_cast<UINT>(TexNum::Alloc::RESULT), { 0, 0 }));

	// �X�v���C�g�̒������擾
	float center_x = game_over_->GetTexSize().x / 2;
	float center_y = game_over_->GetTexSize().y / 2;

	// ��ʂ̒����ɔz�u
	game_over_->SetPosition({ Win32App::window_center_x_ - center_x,
					   Win32App::window_center_y_ - center_y });

	// �{�[�h�̏�����
	BackBoardManager::GetInstance()->Initialize();
}

void QDResultScene::Finalize() {
}

void QDResultScene::Update() {

	// �{�[�h�̍X�V
	BackBoardManager::GetInstance()->Update();

	// �E�X�y�[�X������
	// �E�p�b�hA������
	if (KeyboardInput::TriggerKey(DIK_SPACE) || ControllerInput::TriggerButton(DIB_A)) {

		// �^�C�g����
		SceneManager::SetScene(Scene::TITLE);
	}
}

void QDResultScene::Draw() {

	Object3d::PreDraw();

	// �{�[�h�̕`��
	BackBoardManager::GetInstance()->Draw();

	Sprite::PreDraw();

	// UI�̕`��
	game_over_->Draw();
}

void QDResultScene::DebugDraw() {
}
