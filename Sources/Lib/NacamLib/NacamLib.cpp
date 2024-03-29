#include "NacamLib.h"
#include <wrl.h>
#include "../DirectX/DrawProc.h"
#include "../Audio/AudioManager.h"
#include "../Input/InputManager.h"
#include "../Input/KeyboardInput.h"
#include "../Input/ControllerInput.h"
#include "../Input/NcmInput.h"
#include "../DebugUtill/NcmImGui.h"
#include "../2D/NcmSprite.h"
#include "../3D/Object3d.h"
#include "../3D/IndirectObject3d.h"
#include "../3D/Line.h"
#include "../3D/FbxLoader.h"
#include "../3D/FbxObject3d.h"
#include "../3D/Point.h"
#include "../3D/PlatePoly.h"
#include "../3D/NcmPlatePoly.h"
#include "../Math/Easing/NcmEasing.h"
#include "../FpsManager/FPSManager.h"
#include "../../App/Scene/AbsScene.h"
#include "../../App/Grid/GridRender.h"
#include "../../App/Scene/TemporaryScene.h"
#include "../../App/Scene/SceneManager/SceneManager.h"

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

	// メモリリークを検知して出力してくれるはず
	_CrtDumpMemoryLeaks();
}

void NacamLib::NacamLib_Update()
{
	/*-- fps制御 --*/
	FpsManager::RegulateFps(60);

	/*-- シーンチェンジ --*/
	if (SceneManager::NoticeChangeScene())
	{
		SceneManager::ExecuteSceneChange();
	}

	/*-- キーボード入力更新処理 --*/
	KeyboardInput::Update();

	/*-- コントローラー更新処理 --*/
	ControllerInput::Update();
	NcmInput::Update();

	/*-- シーン更新 --*/
	SceneManager::GetSceneStack().top()->Update();
}

void NacamLib::NacamLib_Draw()
{
	// レンダーテクスチャへの描画
	post_effect_scene_->PreDrawScene();
	SceneManager::GetSceneStack().top()->Draw();
	post_effect_scene_->PostDrawScene();

	/*-- 描画前処理 --*/
	DrawProc::PreDraw(dx_base_);

	/*-- 描画 --*/
	post_effect_scene_->Draw();

	DrawProc::ClearDepthBuffer();
	SceneManager::GetSceneStack().top()->AfterPostEffectDraw();

	// デバッグ時描画
//#ifdef _DEBUG

	/*-- ImGuiの描画前処理 --*/
	NcmImGui::PreDraw();

	/*-- デバッグ描画 --*/
	SceneManager::GetSceneStack().top()->DebugDraw();
	post_effect_scene_->DebugDraw();
	SceneManager::DebugDraw();

	/*-- ImGuiの描画 --*/
	NcmImGui::Draw(cmd_list_.Get());

	//#endif

		/*-- 描画後処理 --*/
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
	using enum PipelineName;

	pipeline_mgr_ = std::make_unique<PipelineManager>();
	pipeline_mgr_->SetTemplateConfigs();

	// 設定が存在する全てのパイプラインを生成
	for (int i = 0; i < (int)(MaxPipelineNum); i++)
	{
		pipeline_mgr_->GeneratePipeline(static_cast<PipelineName>(i));
	}

	PreDraw::StaticInitialize(*pipeline_mgr_);
}

void NacamLib::InputInitialize()
{
	// Input系初期化
	InputManager::GetInstance()->StaticInitialize();
	KeyboardInput::GetInstance()->Initialize();
	ControllerInput::GetInstance()->Initialize();
}

void NacamLib::GameObjectInitialize()
{
	// Model, Object3d, Sprite, AudioMgr自体の初期化
	Object3d::StaticInitialize(device_.Get(), cmd_list_.Get());
	Model::StaticInitialize();
	NcmSprite::StaticInitialize(device_.Get(), cmd_list_.Get());
	AudioManager::StaticInitialize();
	Line::StaticInitialize();
	IndirectObject3d::StaticInitialize(*pipeline_mgr_);
	Point::StaticInitialize(device_.Get(), cmd_list_.Get());
	PlatePoly::StaticInitialize(device_.Get(), cmd_list_.Get());
	NcmPlatePoly::StaticInitialize(device_.Get(), cmd_list_.Get());
	GridRender::StaticInitialize(device_.Get(), cmd_list_.Get());

	NcmEasing::StaticInit();

	// FBXLoader初期化
	FbxLoader::GetInstance()->Initialize(device_.Get());

	// デバイスをセット
	FbxObject3d::SetDevice(device_.Get());
	// グラフィクスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
}

void NacamLib::ImGuiInitialize()
{
	// ImGui初期化
	NcmImGui::Initialize(device_.Get());
}

void NacamLib::SceneInitialize(SceneName initial_scene_name)
{
	// シーンを生成
	SceneManager::SetInitialScene(initial_scene_name);
}

bool NacamLib::CatchQuitSignal()
{
	// 終了命令がきたら || ESCを押したら
	if (!Win32App::ProcessMessage() || KeyboardInput::TriggerKey(DIK_ESCAPE))
	{
		return true;
	}

	return false;
}

void NacamLib::DetectNonStopError(bool should_activate)
{
	// 無効にするなら
	if (!should_activate)
	{
		// その後の処理をスルー
		return;
	}

	// 以下コピペ
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dred_settings;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dred_settings))))
	{
		dred_settings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dred_settings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
}
