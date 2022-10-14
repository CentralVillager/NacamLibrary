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
	ComPtr<ID3DBlob> vs_blob;		// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> ps_blob;		// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> error_blob;	// �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(configs_[(int)(p_name)].VS_name, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vs_blob, &error_blob);

	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(configs_[(int)(p_name)].PS_name, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ps_blob, &error_blob);

	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = configs_[(int)(p_name)].fill_mode;

	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blend_desc{};
	blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blend_desc.BlendEnable = true;
	blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	for (UINT i = 0; i < configs_[(int)(p_name)].num_render_targets; i++)
	{
		gpipeline.BlendState.RenderTarget[i] = blend_desc;
	}
	/*gpipeline.BlendState.RenderTarget[0] = blend_desc;
	gpipeline.BlendState.RenderTarget[1] = blend_desc;*/

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = &configs_[(int)(p_name)].input_layout.front();
	gpipeline.InputLayout.NumElements = (UINT)(configs_[(int)(p_name)].input_layout.size());

	// �}�`�̌`��ݒ�
	gpipeline.PrimitiveTopologyType = configs_[(int)(p_name)].primitive_topology_type;

	//gpipeline.NumRenderTargets = 2;	// �`��Ώۂ�2��
	//gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	//gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.NumRenderTargets = configs_[(int)(p_name)].num_render_targets;
	for (size_t i = 0; i < configs_[(int)(p_name)].rtv_formats.size(); i++)
	{
		gpipeline.RTVFormats[i] = configs_[(int)(p_name)].rtv_formats[i];
	}
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	//CD3DX12_DESCRIPTOR_RANGE desc_range_srv;
	//desc_range_srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	/*CD3DX12_ROOT_PARAMETER rootparams[3]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &desc_range_srv, D3D12_SHADER_VISIBILITY_ALL);*/

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc;
	//root_signature_desc.Init_1_0(_countof(rootparams), rootparams, 1, &sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	root_signature_desc.Init_1_0((UINT)(configs_[(int)(p_name)].root_parameter.size()), &configs_[(int)(p_name)].root_parameter[0], 1, &sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// �o�[�W������������̃V���A���C�Y
	ComPtr<ID3DBlob> root_sig_blob;
	result = D3DX12SerializeVersionedRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &root_sig_blob, &error_blob);

	// ���[�g�V�O�l�`���̐���
	result = device_->CreateRootSignature(0, root_sig_blob->GetBufferPointer(), root_sig_blob->GetBufferSize(), IID_PPV_ARGS(&pipeline_container_[(int)(p_name)].root_signature));

	gpipeline.pRootSignature = pipeline_container_[(int)(p_name)].root_signature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_container_[(int)(p_name)].pipeline_state));

	pipeline_container_[(int)(p_name)].primitive_topology = configs_[(int)(p_name)].primitive_topology;
}

void PipelineManager::SetTemplateConfigs()
{
	using enum PipelineName;

	// Object3d
	PipelineName p_name = Object3d;
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
	configs_[(int)(p_name)].VS_name = configs_[(int)(Object3d)].VS_name;
	configs_[(int)(p_name)].PS_name = configs_[(int)(Object3d)].PS_name;
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_WIREFRAME;
	configs_[(int)(p_name)].input_layout = configs_[(int)(Object3d)].input_layout;
	configs_[(int)(p_name)].primitive_topology_type = configs_[(int)(Object3d)].primitive_topology_type;
	configs_[(int)(p_name)].primitive_topology = configs_[(int)(Object3d)].primitive_topology;
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
	configs_[(int)(p_name)].VS_name = configs_[(int)(IndirectObject3d)].VS_name;
	configs_[(int)(p_name)].PS_name = configs_[(int)(IndirectObject3d)].PS_name;
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_WIREFRAME;
	configs_[(int)(p_name)].input_layout = configs_[(int)(IndirectObject3d)].input_layout;
	configs_[(int)(p_name)].primitive_topology_type = configs_[(int)(IndirectObject3d)].primitive_topology_type;
	configs_[(int)(p_name)].primitive_topology = configs_[(int)(IndirectObject3d)].primitive_topology;
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
	configs_[(int)(p_name)].VS_name = L"Resources/shaders/LineVS.hlsl";
	configs_[(int)(p_name)].PS_name = L"Resources/shaders/LinePS.hlsl";
	configs_[(int)(p_name)].fill_mode = D3D12_FILL_MODE_SOLID;
	configs_[(int)(p_name)].input_layout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	configs_[(int)(p_name)].primitive_topology_type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	configs_[(int)(p_name)].primitive_topology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	configs_[(int)(p_name)].num_render_targets = 1;
	configs_[(int)(p_name)].rtv_formats.resize(1);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(2);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

	// Sprite
	p_name = Sprite;
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
	configs_[(int)(p_name)].num_render_targets = 1;
	configs_[(int)(p_name)].rtv_formats.resize(1);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(1);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].root_parameter.resize(2);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);

	// PostEffect
	p_name = PostEffect;
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
	configs_[(int)(p_name)].num_render_targets = 1;
	configs_[(int)(p_name)].rtv_formats.resize(1);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(2);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].desc_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[1], D3D12_SHADER_VISIBILITY_ALL);

	// AverageBlur
	p_name = AverageBlur;
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
	configs_[(int)(p_name)].num_render_targets = 1;
	configs_[(int)(p_name)].rtv_formats.resize(1);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(2);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].desc_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[1], D3D12_SHADER_VISIBILITY_ALL);

	// GaussianBlur
	p_name = GaussianBlur;
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
	configs_[(int)(p_name)].num_render_targets = 1;
	configs_[(int)(p_name)].rtv_formats.resize(1);
	configs_[(int)(p_name)].rtv_formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	configs_[(int)(p_name)].desc_range.resize(2);
	configs_[(int)(p_name)].desc_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	configs_[(int)(p_name)].desc_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	configs_[(int)(p_name)].root_parameter.resize(3);
	configs_[(int)(p_name)].root_parameter[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[1].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[0], D3D12_SHADER_VISIBILITY_ALL);
	configs_[(int)(p_name)].root_parameter[2].InitAsDescriptorTable(1, &configs_[(int)(p_name)].desc_range[1], D3D12_SHADER_VISIBILITY_ALL);
}
