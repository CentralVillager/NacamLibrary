#pragma once
#include <memory>
#include "../DirectX/DirectXBase.h"
#include "../DirectX/PipelineManager.h"
#include "../DirectX/PreDraw.h"
#include "../PostEffect/PostEffect.h"
#include "../../App/Scene/SceneManager/SceneManager.h"
#include "../../App/Singleton/Singleton.h"

class NacamLib  final : public Singleton<NacamLib>
{
	friend Singleton<NacamLib>;

public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	DirectXBase *dx_base_ = DirectXBase::GetInstance();
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	std::unique_ptr<PostEffect> post_effect_scene_;

	std::unique_ptr<PipelineManager> pipeline_mgr_;

private:

	~NacamLib() override;

public:

	/* 絶対に必要な関数 */

	/// <summary>
	/// 初期化処理
	/// </summary>
	void NacamLib_Initialize(SceneName initial_scene_name);

	/// <summary>
	/// 終了処理
	/// </summary>
	void NacamLib_Finalize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void NacamLib_Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void NacamLib_Draw();

private:

	void Win32AppInitialize();
	void DirectXInitialize();
	void PipelineInitialize();
	void InputInitialize();
	void GameObjectInitialize();
	void ImGuiInitialize();
	void SceneInitialize(SceneName initial_scene_name);

public:

	/// <summary>
	/// 終了命令を受け取る
	/// </summary>
	/// <returns></returns>
	bool CatchQuitSignal();

	/// <summary>
	/// 通常止まらないエラーを強制的に止めるか
	/// </summary>
	/// <param name="should_activate">有効にするならtrue</param>
	void DetectNonStopError(bool should_activate);
};

