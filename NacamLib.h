#pragma once
#include "DirectXBase.h"
#include "Singleton.h"
#include <memory>

#include "PostEffect.h"

class NacamLib  final : public Singleton<NacamLib> {
	friend Singleton<NacamLib>;

public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	DirectXBase *dx_base_ = DirectXBase::GetInstance();
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	std::unique_ptr<PostEffect> post_effect_scene_;

private:

	~NacamLib() override;

public:

	/* 絶対に必要な関数 */

	void NacamLib_Initialize();
	void NacamLib_Finalize();
	void NacamLib_Update();
	void NacamLib_Draw();
	void NacamLib_PostDraw();
	void NacamLib_DebugDraw();

private:

	void WindowInitialize();
	void DirectXInitialize();
	void InputInitialize();
	void GameObjectInitialize();
	void ImGuiInitialize();
	void SceneInitialize();

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

