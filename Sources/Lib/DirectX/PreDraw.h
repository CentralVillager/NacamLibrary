#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "../DirectX/PipelineManager.h"
#include <memory>

class PreDraw
{
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
	/// グラフィクスパイプラインをセット
	/// </summary>
	/// <param name="p_name"></param>
	static void SetPipeline(PipelineName p_name);
};

