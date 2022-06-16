#pragma once
#include <d3dx12.h>
#include <d3dcommon.h>
#include <Windows.h>
#include <wrl.h>

class DirectXBase;

class PipelineManager {

public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	static DirectXBase *dx_base_;
	static ComPtr<ID3D12Device> device_;
	// ルートシグネチャ
	//ComPtr<ID3D12RootSignature> root_signature_;
	// パイプラインステート
	//ComPtr<ID3D12PipelineState> pipeline_state_;

	//ComPtr<ID3DBlob> object_vs_blob_;	// 頂点シェーダオブジェクト
	//ComPtr<ID3DBlob> sprite_vs_blob_;	// 頂点シェーダオブジェクト
	//ComPtr<ID3DBlob> object_ps_blob_;	// ピクセルシェーダオブジェクト
	//ComPtr<ID3DBlob> sprite_ps_blob_;	// ピクセルシェーダオブジェクト
	//ComPtr<ID3DBlob> error_blob_;		// エラーオブジェクト

	//D3D12_INPUT_ELEMENT_DESC inputLayout[3];
	//D3D12_INPUT_ELEMENT_DESC inputLayoutForSprite[2];

	//ComPtr<ID3D12PipelineState> pipelinestate;
	//CD3DX12_DESCRIPTOR_RANGE descRangeCBV, descRangeSRV;
	//CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	//CD3DX12_STATIC_SAMPLER_DESC samplerDesc;
	//ComPtr<ID3D12RootSignature> rootsignature;
	//CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	//ComPtr<ID3DBlob> rootSigBlob;
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

public:

	struct PipelineSet {

		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipeline_state;
		// ルートシグネチャ	
		ComPtr<ID3D12RootSignature> root_signature;
	};

	PipelineManager();

	const D3D12_GRAPHICS_PIPELINE_STATE_DESC &GetGpipeline();

#pragma region 3D
	PipelineSet GeneratePipelineFor3DObj(ID3D12Device *dev);

	static PipelineSet GenerateGraphicsPipeline();

	void LoadAndCompileVS();
	void LoadAndCompilePS();
	void DoSettingPipelineDetail();

	void SettingVSLayoutArray();
	void SettingPipelineState();
#pragma endregion

#pragma region sprite
	PipelineSet GeneratePipelineForSprite(ID3D12Device *dev);

	static PipelineSet GenerateGraphicsPipelineForSprite();

	void LoadAndCompileVSForSprite();
	void LoadAndCompilePSForSprite();
	void DoSettingPipelineDetailForSprite();

	void SettingVSLayoutArrayForSprite();
	void SettingPipelineStateForSprite();
#pragma endregion 

#pragma region common
	void SettingDescRange();
	void SettingRootParam();
	void SettingTexSampler();
	void SettingRootSignature();

	void GenerateRootSignature(ID3D12Device *dev, PipelineSet &pipeline_set_for_sprite);
	void GenerateGraphicsPipelineState(ID3D12Device *dev, PipelineSet &pipeline_set_for_sprite);
#pragma endregion
};
