#include "NacamLib.h"
#include <wrl.h>
#include "../Sprite/NcmSprite.h"
#include "../Audio/AudioManager.h"
#include "../Input/InputManager.h"
#include "../Input/KeyboardInput.h"
#include "../Input/ControllerInput.h"
#include "../Input/NcmInput.h"
#include "../Object3d/Object3d.h"
#include "../IndirectObject3d/IndirectObject3d.h"
#include "../Line/Line.h"
#include "../Fbx/FbxLoader.h"
#include "../Fbx/FbxObject3d.h"
#include "../DrawProc/DrawProc.h"
#include "../../App/Debug/NcmImGui.h"
#include "../../App/Scene/SceneManager/SceneManager.h"
#include "../../App/FpsManager/FPSManager.h"
#include "../../App/Scene/AbsScene.h"
#include "../../App/Scene/TemporaryScene.h"
#include "../Sources/App/Math/Easing/NcmEasing.h"
#include "../Point/Point.h"
#include "../PlatePoly/PlatePoly.h"
#include "../PlatePoly/NcmPlatePoly.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

NacamLib::~NacamLib()
{
	ControllerInput::Finalize();
	KeyboardInput::Finalize();
	NcmSprite::TermSprite();
}

void NacamLib::NacamLib_Initialize(SceneName initial_scene_name)
{
	Win32AppInitialize();
	DirectXInitialize();
	PipelineInitialize();
	InputInitialize();
	GameObjectInitialize();
	ImGuiInitialize();
	SceneInitialize(initial_scene_name);

	post_effect_scene_ = std::make_unique<PostEffect>();
	post_effect_scene_->Initialize();
}

void NacamLib::NacamLib_Finalize()
{
	FbxLoader::GetInstance()->Finalize();
	NcmImGui::Term();

	UnregisterClass(Win32App::GetW().lpszClassName, Win32App::GetW().hInstance);

	// ���������[�N�����m���ďo�͂��Ă����͂�
	_CrtDumpMemoryLeaks();
}

void NacamLib::NacamLib_Update(int fps)
{
	/*-- fps���� --*/
	FpsManager::RegulateFps(fps);

	/*-- �V�[���`�F���W --*/
	if (SceneManager::NoticeChangeScene())
	{
		SceneManager::ExecuteSceneChange();
	}

	/*-- �L�[�{�[�h���͍X�V���� --*/
	KeyboardInput::Update();

	/*-- �R���g���[���[�X�V���� --*/
	ControllerInput::Update();
	NcmInput::Update();

	/*-- �V�[���X�V --*/
	SceneManager::GetSceneStack().top()->Update();
}

void NacamLib::NacamLib_Draw()
{
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

	/*-- ImGui�̕`��O���� --*/
	NcmImGui::PreDraw();

	/*-- �f�o�b�O�`�� --*/
	SceneManager::GetSceneStack().top()->DebugDraw();
	post_effect_scene_->DebugDraw();
	SceneManager::DebugDraw();

	/*-- ImGui�̕`�� --*/
	NcmImGui::Draw(cmd_list_.Get());

//#endif

	/*-- �`��㏈�� --*/
	DrawProc::PostDraw(dx_base_);
}

void NacamLib::Win32AppInitialize()
{
	Win32App::StaticInitialize();
}

void NacamLib::DirectXInitialize()
{
	dx_base_->Initialize();

	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();
	DrawProc::StaticInitialize();
}

void NacamLib::PipelineInitialize()
{
	pipeline_mgr_ = std::make_unique<PipelineManager>();
	pipeline_mgr_->SetTemplateConfigs();
	pipeline_mgr_->GeneratePipeline(PipelineName::Object3d);
	pipeline_mgr_->GeneratePipeline(PipelineName::Object3d_WireFrame);
	pipeline_mgr_->GeneratePipeline(PipelineName::IndirectObject3d);
	pipeline_mgr_->GeneratePipeline(PipelineName::IndirectObject3d_WireFrame);
	pipeline_mgr_->GeneratePipeline(PipelineName::Line);
	pipeline_mgr_->GeneratePipeline(PipelineName::Point);
	pipeline_mgr_->GeneratePipeline(PipelineName::PlatePoly);
	pipeline_mgr_->GeneratePipeline(PipelineName::Sprite);
	pipeline_mgr_->GeneratePipeline(PipelineName::PostEffect);
	pipeline_mgr_->GeneratePipeline(PipelineName::AverageBlur);
	pipeline_mgr_->GeneratePipeline(PipelineName::GaussianBlur);

	PreDraw::StaticInitialize(*pipeline_mgr_);
}

void NacamLib::InputInitialize()
{
	// Input�n������
	InputManager::GetInstance()->StaticInitialize();
	KeyboardInput::GetInstance()->Initialize();
	ControllerInput::GetInstance()->Initialize();
}

void NacamLib::GameObjectInitialize()
{
	// Model, Object3d, Sprite, AudioMgr���̂̏�����
	Object3d::StaticInitialize(device_.Get(), cmd_list_.Get());
	Model::StaticInitialize();
	NcmSprite::StaticInitialize(device_.Get(), cmd_list_.Get());
	AudioManager::StaticInitialize();
	Line::StaticInitialize();
	IndirectObject3d::StaticInitialize(*pipeline_mgr_);
	Point::StaticInitialize(device_.Get(), cmd_list_.Get());
	PlatePoly::StaticInitialize(device_.Get(), cmd_list_.Get());
	NcmPlatePoly::StaticInitialize(device_.Get(), cmd_list_.Get());

	NcmEasing::StaticInit();

	// FBXLoader������
	FbxLoader::GetInstance()->Initialize(device_.Get());

	// �f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(device_.Get());
	// �O���t�B�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
}

void NacamLib::ImGuiInitialize()
{
	// ImGui������
	NcmImGui::Initialize(device_.Get());
}

void NacamLib::SceneInitialize(SceneName initial_scene_name)
{
	// �V�[���𐶐�
	SceneManager::SetInitialScene(initial_scene_name);
}

bool NacamLib::CatchQuitSignal()
{
	// �I�����߂������� || ESC����������
	if (!Win32App::ProcessMessage() || KeyboardInput::TriggerKey(DIK_ESCAPE))
	{
		return true;
	}

	return false;
}

void NacamLib::DetectNonStopError(bool should_activate)
{
	// �����ɂ���Ȃ�
	if (!should_activate)
	{
		// ���̌�̏������X���[
		return;
	}

	// �ȉ��R�s�y
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dred_settings;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dred_settings))))
	{
		dred_settings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dred_settings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
}
