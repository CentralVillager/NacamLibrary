#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "../Pipeline/PipelineManager.h"
#include <memory>

class PreDraw {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	PreDraw();
	~PreDraw();

public:

	static void StaticInitialize(const PipelineManager &p_mgr);

private:

	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	// PipelineManagerのインスタンス
	static std::unique_ptr<PipelineManager> pipeline_mgr_;

public:

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="p_name"></param>
	static void PreRender(PipelineName p_name);
};

