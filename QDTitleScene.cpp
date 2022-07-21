#include "QDTitleScene.h"
#include "Win32App.h"
#include "SceneManager.h"
#include "BackBoardManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"

QDTitleScene::QDTitleScene() {

	// �J�����̐���
	camera_ = make_unique<Camera>();

	// �T�E���h�̏�����
	bgm_ = make_unique<AudioManager>();
	bgm_->LoadWaveFile("Resources/Sounds/", "main");

	// UI�̓ǂݍ���
	Sprite::LoadTexture(static_cast<UINT>(TexNum::Alloc::QUANTUM), L"Resources/Textures/QuantumDrive_ReRe.png");

	// �`��f�[�^�̐���
	quantum_drive_ = make_unique<Sprite>();
}

QDTitleScene::~QDTitleScene() {
}

void QDTitleScene::Initialize() {

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	Object3d::SetCamera(camera_.get());
	FbxObject3d::SetCamera(camera_.get());

	// �`��f�[�^�̑��
	quantum_drive_ = static_cast<unique_ptr<Sprite>>(Sprite::Create(static_cast<UINT>(TexNum::Alloc::QUANTUM), { 0, 0 }));

	// ���S�����
	float center_x = quantum_drive_->GetTexSize().x / 2;
	float center_y = quantum_drive_->GetTexSize().y / 2;

	// ��ʂ̒��S�ɔz�u
	quantum_drive_->SetPosition({ Win32App::window_center_x_ - center_x,
					   Win32App::window_center_y_ - center_y });

	// �����̍Đ�
	//main_->PlayWave(0.1f, AudioManager::LOOP, false);

	// �{�[�h�̏�����
	BackBoardManager::GetInstance()->Initialize();
}

void QDTitleScene::Finalize() {

	// BGM���~�߂�
	bgm_->StopWave();

	// �A�����[�h����
	bgm_->UnloadWaveFile();
}

void QDTitleScene::Update() {

	// �{�[�h�̍X�V
	BackBoardManager::GetInstance()->Update();

	// �E�X�y�[�X������
	// �E�p�b�hA������
	if (KeyboardInput::TriggerKey(DIK_SPACE) || ControllerInput::TriggerButton(DIB_A)) {

		// �I������
		Finalize();

		// ���C����
		SceneManager::SetScene(Scene::MAIN);
	}
}

void QDTitleScene::Draw() {

	Object3d::PreDraw();

	// �{�[�h�̕`��
	BackBoardManager::GetInstance()->Draw();

	Sprite::PreDraw();

	// UI�̕`��
	quantum_drive_->Draw();
}

void QDTitleScene::DebugDraw() {
}
