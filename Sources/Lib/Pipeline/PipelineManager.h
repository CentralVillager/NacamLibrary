#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <array>
#include "../DirectXBase/DirectXBase.h"
#include <memory>

/// <summary>
/// パイプラインの名前
/// </summary>
enum class PipelineName
{
	Object3d,
	Object3d_WireFrame,
	IndirectObject3d,
	IndirectObject3d_WireFrame,
	Line,
	Point,
	PlatePoly,
	Sprite,

	//Primitive,

	PostEffect,
	AverageBlur,
	GaussianBlur,

	MaxPipelineNum
};

struct PipelineSet
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> root_signature;
	ComPtr<ID3D12PipelineState> pipeline_state;
	D3D_PRIMITIVE_TOPOLOGY primitive_topology;
};

/// <summary>
/// パイプライン毎の詳細設定
/// </summary>
struct PipelineConfigs
{
	LPCWSTR pipeline_name;
	LPCWSTR VS_name;
	LPCWSTR PS_name;
	LPCWSTR GS_name;
	LPCWSTR CS_name;
	D3D12_FILL_MODE fill_mode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> input_layout;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive_topology_type;
	D3D_PRIMITIVE_TOPOLOGY primitive_topology;
	UINT num_render_targets;
	std::vector<DXGI_FORMAT> rtv_formats;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> desc_range;
	std::vector<CD3DX12_ROOT_PARAMETER> root_parameter;
};

class PipelineManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	PipelineManager();
	~PipelineManager();

private:

	// デバイス
	static ComPtr<ID3D12Device> device_;

	// パイプラインコンテナ
	static std::array<PipelineSet, (int)(PipelineName::MaxPipelineNum)> pipeline_container_;

	// パイプライン設定集
	static std::array<PipelineConfigs, (int)(PipelineName::MaxPipelineNum)> configs_;

public:

	inline const PipelineSet &GetPipeline(PipelineName p_name) { return pipeline_container_[(int)(p_name)]; }

	/// <summary>
	/// グラフィクスパイプラインの生成
	/// </summary>
	/// <param name="p_name"></param>
	void GeneratePipeline(const PipelineName &p_name);

	/// <summary>
	/// パイプライン設定の設定
	/// </summary>
	void SetTemplateConfigs();
};
