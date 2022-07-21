#include "NacamLib.h"
#include <wrl.h>
#include "InputManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "Object3d.h"
#include "Sprite.h"
#include "AudioManager.h"
#include "PrimitiveObject.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "IndirectObject3d.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "DrawProc.h"
#include "FPSManager.h"
#include "AbstractScene.h"
#include "QDSplashScene.h"
#include "QDTitleScene.h"
#include "QDMainScene.h"
#include "TemporaryScene.h"
#include "QDResultScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

NacamLib::~NacamLib() {

	ControllerInput::Finalize();
	KeyboardInput::Finalize();
}

void NacamLib::NacamLib_Initialize(Scene initial_scene_name) {

	Win32AppInitialize();
	DirectXInitialize();
	InputInitialize();
	GameObjectInitialize();
	ImGuiInitialize();
	SceneInitialize(initial_scene_name);

	post_effect_scene_ = std::make_unique<PostEffect>();
	post_effect_scene_->Initialize();
}

void NacamLib::NacamLib_Finalize() {

	FbxLoader::GetInstance()->Finalize();

	UnregisterClass(Win32App::GetW().lpszClassName, Win32App::GetW().hInstance);

	// ���������[�N�����m���ďo�͂��Ă����͂�
	_CrtDumpMemoryLeaks();
}

void NacamLib::NacamLib_Update(int fps) {

	/*-- fps���� --*/
	FpsManager::RegulateFps(fps);

	/*-- �L�[�{�[�h���͍X�V���� --*/
	KeyboardInput::Update();

	/*-- �R���g���[���[�X�V���� --*/
	ControllerInput::Update();

	/*-- �V�[���X�V --*/
	SceneManager::GetSceneStack().top()->Update();
}

void NacamLib::NacamLib_Draw() {

	// �����_�[�e�N�X�`���ւ̕`��
	post_effect_scene_->PreDrawScene();
	SceneManager::GetSceneStack().top()->Draw();
	post_effect_scene_->PostDrawScene();

	/*-- �`��O���� --*/
	DrawProc::PreDraw(dx_base_);

	/*-- �`�� --*/
	post_effect_scene_->Draw();

	// �f�o�b�O���`��
//#ifdef _DEBUG

	/*-- �^�C�g���o�[�Ɍ�fps����`�� --*/
	FpsManager::ObserveFps();

	/*-- ImGui�̕`��O���� --*/
	ImGuiManager::PreDraw();

	/*-- �f�o�b�O�`�� --*/
	SceneManager::GetSceneStack().top()->DebugDraw();

	post_effect_scene_->DebugDraw();

	/*-- ImGui�̕`�� --*/
	ImGuiManager::Draw(cmd_list_.Get());

	//#endif

	/*-- �`��㏈�� --*/
	DrawProc::PostDraw(dx_base_);
}

void NacamLib::Win32AppInitialize() {

	Win32App::StaticInitialize();
}

void NacamLib::DirectXInitialize() {

	dx_base_->Initialize();

	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();
	DrawProc::StaticInitialize();
}

void NacamLib::InputInitialize() {

	// Input�n������
	InputManager::GetInstance()->StaticInitialize();
	KeyboardInput::GetInstance()->Initialize();
	ControllerInput::GetInstance()->Initialize();
}

void NacamLib::GameObjectInitialize() {

	// Model, Object3d, Sprite, AudioMgr���̂̏�����
	Object3d::StaticInitialize(device_.Get(), cmd_list_.Get());
	Model::StaticInitialize();
	Sprite::StaticInitialize(device_.Get(), cmd_list_.Get(), Win32App::GetWindowWidth(), Win32App::GetWindowHeight());
	AudioManager::StaticInitialize();
	PrimitiveObject::StaticInitialize(device_.Get(), cmd_list_.Get());
	//IndirectObject3d::StaticInitialize(device_.Get(), cmd_list_.Get());

	// FBXLoader������
	FbxLoader::GetInstance()->Initialize(device_.Get());

	// �f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(device_.Get());
	// �O���t�B�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
}

void NacamLib::ImGuiInitialize() {

	// ImGui������
	ImGuiManager::Initialize(device_.Get());
}

void NacamLib::SceneInitialize(Scene initial_scene_name) {

	// �V�[��������
	SceneManager::ClearSceneStack();

	// �V�[���𐶐�
	SceneManager::SetScene(initial_scene_name);
}

bool NacamLib::CatchQuitSignal() {

	// �I�����߂������� || ESC����������
	if (!Win32App::ProcessMessage() || KeyboardInput::TriggerKey(DIK_ESCAPE)) {

		return true;
	}

	return false;
}

void NacamLib::DetectNonStopError(bool should_activate) {

	// �����ɂ���Ȃ�
	if (!should_activate) {

		// ���̌�̏������X���[
		return;
	}

	// �ȉ��R�s�y
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dred_settings;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dred_settings)))) {

		dred_settings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dred_settings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
}
