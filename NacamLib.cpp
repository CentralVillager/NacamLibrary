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
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "DrawProc.h"
#include "FPSManager.h"
#include "AbstractScene.h"
#include "SplashScene.h"
#include "TitleScene.h"
#include "MainScene.h"
#include "TemporaryScene.h"
#include "ResultScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

NacamLib::~NacamLib() {

	ControllerInput::Finalize();
	KeyboardInput::Finalize();
}

void NacamLib::NacamLib_Initialize() {

	WindowInitialize();
	DirectXInitialize();
	InputInitialize();
	GameObjectInitialize();
	ImGuiInitialize();
	SceneInitialize();

	post_effect_scene_ = std::make_unique<PostEffect>();
	post_effect_scene_->Initialize();
}

void NacamLib::NacamLib_Finalize() {

	FbxLoader::GetInstance()->Finalize();

	UnregisterClass(Window::GetW().lpszClassName, Window::GetW().hInstance);
}

void NacamLib::NacamLib_Update() {

	/*-- キーボード入力更新処理 --*/
	KeyboardInput::Update();

	/*-- コントローラー更新処理 --*/
	ControllerInput::Update();

	/*-- シーン更新 --*/
	SceneManager::GetSceneStack().top()->Update();
}

void NacamLib::NacamLib_Draw() {

	// レンダーテクスチャへの描画
	post_effect_scene_->PreDrawScene();
	SceneManager::GetSceneStack().top()->Draw();
	post_effect_scene_->PostDrawScene();

	/*-- 描画前処理 --*/
	DrawProc::PreDraw(dx_base_);

	/*-- 描画 --*/
	post_effect_scene_->Draw();
}

void NacamLib::NacamLib_PostDraw() {

	/*-- 描画後処理 --*/
	DrawProc::PostDraw(dx_base_);
}

void NacamLib::NacamLib_DebugDraw() {

	/*-- タイトルバーに現fps数を描画 --*/
	FpsManager::ObserveFps();

	/*-- ImGuiの描画前処理 --*/
	ImGuiManager::PreDraw();

	/*-- デバッグ描画 --*/
	SceneManager::GetSceneStack().top()->DebugDraw();

	/*-- ImGuiの描画 --*/
	ImGuiManager::Draw(cmd_list_.Get());
}

void NacamLib::WindowInitialize() {

	Window::StaticInitialize();
}

void NacamLib::DirectXInitialize() {

	dx_base_->Initialize();

	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	cmd_list_ = DirectXBase::GetInstance()->GetCmdList().Get();
}

void NacamLib::InputInitialize() {

	// Input系初期化
	InputManager::GetInstance()->StaticInitialize();
	KeyboardInput::GetInstance()->Initialize();
	ControllerInput::GetInstance()->Initialize();
}

void NacamLib::GameObjectInitialize() {

	// Model, Object3d, Sprite, AudioMgr自体の初期化
	Object3d::StaticInitialize(device_.Get(), cmd_list_.Get());
	Model::StaticInitialize();
	Sprite::StaticInitialize(device_.Get(), cmd_list_.Get(), Window::GetWindowWidth(), Window::GetWindowHeight());
	AudioManager::StaticInitialize();
	PrimitiveObject::StaticInitialize(device_.Get(), cmd_list_.Get());

	// FBXLoader初期化
	FbxLoader::GetInstance()->Initialize(device_.Get());

	// デバイスをセット
	FbxObject3d::SetDevice(device_.Get());
	// グラフィクスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
}

void NacamLib::ImGuiInitialize() {

	// ImGui初期化
	ImGuiManager::Initialize(device_.Get());
}

void NacamLib::SceneInitialize() {

	// シーン初期化
	SceneManager::ClearSceneStack();
}

bool NacamLib::CatchQuitSignal() {

	// 終了命令がきたら || ESCを押したら
	if (!Window::ProcessMessage() || KeyboardInput::TriggerKey(DIK_ESCAPE)) {

		return true;
	}

	return false;
}

void NacamLib::DetectNonStopError(bool should_activate) {

	// 無効にするなら
	if (!should_activate) {

		// その後の処理をスルー
		return;
	}

	// 以下コピペ
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dred_settings;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dred_settings)))) {

		dred_settings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dred_settings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
}
