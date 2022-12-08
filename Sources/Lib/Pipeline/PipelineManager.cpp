#include "../Pipeline/PipelineManager.h"
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "../DirectXBase/DirectXBase.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> PipelineManager::device_;
array<PipelineSet, (int)(PipelineName::MaxPipelineNum)> PipelineManager::pipeline_container_;
array<PipelineConfigs, (int)(PipelineName::MaxPipelineNum)> PipelineManager::configs_;

PipelineManager::PipelineManager()
{
	device_ = DirectXBase::GetInstance()->GetDevice().Get();
}

PipelineManager::~PipelineManager()
{}

void PipelineManager::GeneratePipeline(const PipelineName &p_name)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vs_blob;		// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> ps_blob;		// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gs_blob;		// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> error_blob;	// エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(configs_[(int)(p_name)].VS_name, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vs_blob, &error_blob);

	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(configs_[(int)(p_name)].PS_name, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ps_blob, &error_blob);

	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	if (configs_[(int)(p_name)].GS_name)
	{
		// ジオメトリシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(configs_[(int)(p_name)].GS_name, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "gs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &gs_blob, &error_blob);

		if (FAILED(result))
		{
			// errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(error_blob->GetBufferSize());

			std::copy_n((char *)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
			errstr += "\n";

			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
	}

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());
	if (configs_[(int)(p_name)].GS_name) { gpipeline.GS = CD3DX12_SHADER_BYTECODE(gs_blob.Get()); }

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = configs_[(int)(p_name)].fill_mode;

	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	if (configs_[(int)(p_name)].GS_name) { gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; }

	// ブレンドステートの設定
	for (UINT i = 0; i < configs_[(int)(p_name)].num_render_targets; i++)
	{
		gpipeline.BlendState.RenderTarget[i] = configs_[(int)(p_name)].blend_desc;
	}

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = &configs_[(int)(p_name)].input_layout.front();
	gpipeline.InputLayout.NumElements = (UINT)(configs_[(int)(p_name)].input_layout.size());

	// 図形の形状設定
	gpipeline.PrimitiveTopologyType = configs_[(int)(p_name)].primitive_topology_type;

	// レンダーターゲットの数
	gpipeline.NumRenderTargets = configs_[(int)(p_name)].num_render_targets;
	for (size_t i = 0; i < configs_[(int)(p_name)].rtv_formats.size(); i++)
	{
		gpipeline.RTVFormats[i] = configs_[(int)(p_name)].rtv_formats[i];
	}
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc = CD3DX12_STATIC_SAMPLER_DESC(0);
	sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc;
	root_signature_desc.Init_1_0((UINT)(configs_[(int)(p_name)].root_parameter.size()), &configs_[(int)(p_name)].root_parameter[0], 1, &sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// バージョン自動判定のシリアライズ
	ComPtr<ID3DBlob> root_sig_blob;
	result = D3DX12SerializeVersionedRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &root_sig_blob, &error_blob);
	assert(SUCCEEDED(result));

	// ルートシグネチャの生成
	result = device_->CreateRootSignature(0, root_sig_blob->GetBufferPointer(), root_sig_blob->GetBufferSize(), IID_PPV_ARGS(&pipeline_container_[(int)(p_name)].root_signature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = pipeline_container_[(int)(p_name)].root_signature.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_container_[(int)(p_name)].pipeline_state));
	pipeline_container_[(int)(p_name)].pipeline_state->SetName(configs_[(int)(p_name)].pipeline_name);
	assert(SUCCEEDED(result));

	pipeline_container_[(int)(p_name)].primitive_topology = configs_[(int)(p_name)].primitive_topology;
}

void PipelineManager::SetTemplateConfigs()
{
	using enum PipelineName;

	// Object3d
	PipelineName p_name = Object3d;
	configs_[(int)(p_name)].pipeline_name = L"Object3d";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/BasicVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/BasicPS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout.resize(3);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);

	// Object3d_WireFrame
	p_name = Object3d_WireFrame;
	configs_[(int)(p_name)].pipeline_name = L"Object3d_WireFrame";
	configs_[(int)(p_name)].VS_name = configs_[(int)(Object3d)].VS_name;
	configs_[(int)(p_name)].PS_name = configs_[(int)(Object3d)].PS_name;
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_WIREFRAME;
	configs_[(int)(p_name)].input_layout = configs_[(int)(Object3d)].input_layout;
	configs_[(int)(p_name)].primitive_topology_type = configs_[(int)(Object3d)].primitive_topology_type;
	configs_[(int)(p_name)].primitive_topology = configs_[(int)(Object3d)].primitive_topology;
	configs_[(int)(p_name)].blend_desc = configs_[(int)(Object3d)].blend_desc;
	configs_[(int)(p_name)].num_render_targets = configs_[(int)(Object3d)].num_render_targets;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = configs_[(int)(Object3d)].rtv_formats[0];
	configs_[(int)(p_name)].rtv_formats[1] = configs_[(int)(Object3d)].rtv_formats[1];
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);

	// IndirectObject3d
	p_name = IndirectObject3d;
	configs_[(int)(p_name)].pipeline_name = L"IndirectObject3d";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/IndirectVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/IndirectPS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout.resize(3);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	configs_[(int)(p_name)].blend_desc = configs_[(int)(Object3d)].blend_desc;
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);

	// IndirectObject3d_WireFrame
	p_name = IndirectObject3d_WireFrame;
	configs_[(int)(p_name)].pipeline_name = L"IndirectObject3d_WireFrame";
	configs_[(int)(p_name)].VS_name = configs_[(int)(IndirectObject3d)].VS_name;
	configs_[(int)(p_name)].PS_name = configs_[(int)(IndirectObject3d)].PS_name;
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_WIREFRAME;
	configs_[(int)(p_name)].input_layout = configs_[(int)(IndirectObject3d)].input_layout;
	configs_[(int)(p_name)].primitive_topology_type = configs_[(int)(IndirectObject3d)].primitive_topology_type;
	configs_[(int)(p_name)].primitive_topology = configs_[(int)(IndirectObject3d)].primitive_topology;
	configs_[(int)(p_name)].blend_desc = configs_[(int)(Object3d)].blend_desc;
	configs_[(int)(p_name)].num_render_targets = configs_[(int)(IndirectObject3d)].num_render_targets;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = configs_[(int)(IndirectObject3d)].rtv_formats[0];
	configs_[(int)(p_name)].rtv_formats[1] = configs_[(int)(IndirectObject3d)].rtv_formats[1];
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);

	// Line
	p_name = Line;
	configs_[(int)(p_name)].pipeline_name = L"Line";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/LineVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/LinePS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(2);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

	// Point
	p_name = Point;
	configs_[(int)(p_name)].pipeline_name = L"Point";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/PointVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/PointPS.hlsl";
	configs_[(int)(p_name)].GS_name = L"Resources/shaders/PointGS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_WIREFRAME;
	configs_[(int)(p_name)].input_layout.resize(2);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(2);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);

	// PlatePoly
	p_name = PlatePoly;
	configs_[(int)(p_name)].pipeline_name = L"PlatePoly";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/ParticleVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/ParticlePS.hlsl";
	configs_[(int)(p_name)].GS_name = L"Resources/shaders/ParticleGS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout.resize(2);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	/*configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_ONE;*/
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	/*configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;*/
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(2);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].desc_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(2);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[1], D3D12_SHADER_VISIBILITY_ALL);

	// Sprite
	p_name = Sprite;
	configs_[(int)(p_name)].pipeline_name = L"Sprite";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/SpriteVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/SpritePS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout.resize(2);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	/*configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;*/
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(2);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);

	// PostEffect
	p_name = PostEffect;
	configs_[(int)(p_name)].pipeline_name = L"PostEffect";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/PostEffectVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/PostEffectPS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout.resize(2);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(2);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].desc_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[1], D3D12_SHADER_VISIBILITY_ALL);

	// AverageBlur
	p_name = AverageBlur;
	configs_[(int)(p_name)].pipeline_name = L"AverageBlur";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/BlurVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/BlurPS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout.resize(2);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(2);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].desc_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[1], D3D12_SHADER_VISIBILITY_ALL);

	// GaussianBlur
	p_name = GaussianBlur;
	configs_[(int)(p_name)].pipeline_name = L"GaussianBlur";
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/BlurVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/GaussianBlurPS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout.resize(2);
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	configs_[(int)(p_name)].blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	configs_[(int)(p_name)].blend_desc.BlendEnable = true;
	configs_[(int)(p_name)].blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	configs_[(int)(p_name)].blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	configs_[(int)(p_name)].blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	configs_[(int)(p_name)].blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	configs_[(int)(p_name)].num_render_targets = 2;
	configs_[(int)(p_name)].rtv_formats.resize(2);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].rtv_formats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(2);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].desc_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[1], D3D12_SHADER_VISIBILITY_ALL);
}
