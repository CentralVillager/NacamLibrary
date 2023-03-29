#include <string>
#include <d3dcompiler.h>
#include "../PostEffect/PostEffect.h"
#include "../Win32App/Win32App.h"
#include "../Input/KeyboardInput.h"
#include "../../Lib/Utility/NcmUtil.h"
#include "../../Lib/DebugUtill/NcmImGui.h"
#include "../../Lib/DirectX/PreDraw.h"
#include "../DirectX/DirectXBase.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace NcmUtill;

ComPtr<ID3D12Device> PostEffect::device_ = nullptr;
ComPtr<ID3D12GraphicsCommandList> PostEffect::command_list_;
const float PostEffect::clear_color_[4] = { Convert256to01(25), Convert256to01(25), Convert256to01(25), 0.0f };
XMFLOAT4 PostEffect::color_ = { 1, 1, 1, 1 };

PostEffect::PostEffect() :
	pipeline_(),
	vb_view_()
{
	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	command_list_ = DirectXBase::GetInstance()->GetCommandList().Get();
}

PostEffect::~PostEffect()
{}

void PostEffect::Initialize()
{
	HRESULT result;

	// ���_�o�b�t�@����
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * vertex_num_),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	vertex_buffer_->SetName(L"PostEffectVertexBuffer");

	assert(SUCCEEDED(result));

	// ���_�f�[�^
	Vertex vertices[vertex_num_] =
	{
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},	// ����
		{{-1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},	// ����
		{{+1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},	// �E��
		{{+1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},	// �E��
	};

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result))
	{
		memcpy(vert_map, vertices, sizeof(vertices));
		vertex_buffer_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vb_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vb_view_.SizeInBytes = sizeof(Vertex) * 4;
	vb_view_.StrideInBytes = sizeof(Vertex);

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buffer_));
	const_buffer_->SetName(L"PostEffectConstantBuffer");

	assert(SUCCEEDED(result));

	// �e�N�X�`�����\�[�X����
	CD3DX12_RESOURCE_DESC tex_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		Win32App::SIZE_.x,
		static_cast<UINT>(Win32App::SIZE_.y),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	// �e�N�X�`���o�b�t�@�̐���
	for (int i = 0; i < 2; i++)
	{
		result = device_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&tex_res_desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clear_color_),
			IID_PPV_ARGS(&tex_buff_[i]));
	}

	assert(SUCCEEDED(result));

	// SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srv_desc_heap_desc = {};
	srv_desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srv_desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srv_desc_heap_desc.NumDescriptors = 2;

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device_->CreateDescriptorHeap(&srv_desc_heap_desc, IID_PPV_ARGS(&desc_heap_SRV_));
	desc_heap_SRV_->SetName(L"PostEffectDescriptorHeapSRV");
	assert(SUCCEEDED(result));

	// SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;

	// �f�X�N���v�^�q�[�v��SRV�쐬
	for (int i = 0; i < 2; i++)
	{
		device_->CreateShaderResourceView(tex_buff_[i].Get(),
			&srv_desc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				desc_heap_SRV_->GetCPUDescriptorHandleForHeapStart(), i,
				device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}

	// RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtv_desc_heap_desc{};
	rtv_desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_desc_heap_desc.NumDescriptors = 2;

	// RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device_->CreateDescriptorHeap(&rtv_desc_heap_desc, IID_PPV_ARGS(&desc_heap_RTV_));
	desc_heap_RTV_->SetName(L"PostEffectDescriptorHeapRTV");

	assert(SUCCEEDED(result));

	for (int i = 0; i < 2; i++)
	{
		// �f�X�N���v�^�q�[�v��RTV�쐬
		device_->CreateRenderTargetView(tex_buff_[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				desc_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
				device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
		);
	}
	// �f�v�X�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depth_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Win32App::SIZE_.x,
		Win32App::SIZE_.y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	// �[�x�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depth_res_desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depth_buff_));
	depth_buff_->SetName(L"PostEffectDepthBuffer");

	assert(SUCCEEDED(result));

	// DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc{};
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc_heap_desc.NumDescriptors = 1;

	// DSV�p�f�X�N���v�^�q�[�v���쐬
	result = device_->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&desc_heap_DSV_));
	desc_heap_DSV_->SetName(L"PostEffectDescriptorHeapDSV");

	assert(SUCCEEDED(result));

	// �f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
	dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(depth_buff_.Get(),
		&dsv_desc,
		desc_heap_DSV_->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Draw()
{
	/*if (KeyboardInput::TriggerKey(DIK_1))
	{
		static int tex = 0;

		tex = (tex + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
		srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		device_->CreateShaderResourceView(tex_buff_[tex].Get(),
			&srv_desc,
			desc_heap_SRV_->GetCPUDescriptorHandleForHeapStart()
		);
	}*/

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *constMap = nullptr;
	HRESULT result = this->const_buffer_->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = this->color_;
		constMap->mat = XMMatrixIdentity();	// �s��̍���	
		this->const_buffer_->Unmap(0, nullptr);
	}

	// ImGui�Ŏw�肵���p�C�v���C����ݒ�
	SetPipeline(pipeline_);

	// �v���~�e�B�u�`���ݒ�
	command_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�o�b�t�@�̐ݒ�
	command_list_->IASetVertexBuffers(0, 1, &this->vb_view_);

	ID3D12DescriptorHeap *ppHeaps[] = { desc_heap_SRV_.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	command_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	command_list_->SetGraphicsRootConstantBufferView(0, this->const_buffer_->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	command_list_->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_SRV_->GetGPUDescriptorHandleForHeapStart(), 0,
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	));
	command_list_->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_SRV_->GetGPUDescriptorHandleForHeapStart(), 1,
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	));
	//command_list_->SetGraphicsRootDescriptorTable(1, desc_heap_SRV_->GetGPUDescriptorHandleForHeapStart());
	// �`��R�}���h
	command_list_->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::DebugDraw()
{
	static int button;

	ImGui::Begin("PostEffect", 0, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::RadioButton("Basic", &button, 0))
	{
		pipeline_ = Pipeline::Basic;
	}
	else if (ImGui::RadioButton("AverageBlur", &button, 1))
	{
		pipeline_ = Pipeline::AverageBlur;
	}
	else if (ImGui::RadioButton("GaussianBlur", &button, 2))
	{
		pipeline_ = Pipeline::GaussianBlur;
	}
	else if (ImGui::RadioButton("RadialBlur", &button, 3))
	{
		pipeline_ = Pipeline::RadialBlur;
	}
	ImGui::End();
}

void PostEffect::SetPipeline(Pipeline p)
{
	if (p == Pipeline::Basic)
	{
		PreDraw::SetPipeline(PipelineName::PostEffect);
	}
	else if (p == Pipeline::AverageBlur)
	{
		PreDraw::SetPipeline(PipelineName::AverageBlur);
	}
	else if (p == Pipeline::GaussianBlur)
	{
		PreDraw::SetPipeline(PipelineName::GaussianBlur);
	}
	else if (p == Pipeline::RadialBlur)
	{
		PreDraw::SetPipeline(PipelineName::RadialBlur);
	}
}

void PostEffect::PreDrawScene()
{
	// ���\�[�X�o���A��ύX�i�V�F�[�_�[���\�[�X->�`��\�j
	for (int i = 0; i < 2; i++)
	{
		command_list_->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(tex_buff_[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET)
		);
	}
	// �����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_h[2];
	for (int i = 0; i < 2; i++)
	{
		rtv_h[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			desc_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	// �f�v�X�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsv_h = desc_heap_DSV_->GetCPUDescriptorHandleForHeapStart();

	// �����_�[�^�[�Q�b�g���Z�b�g
	command_list_->OMSetRenderTargets(2, rtv_h, false, &dsv_h);

	CD3DX12_VIEWPORT viewports[2];
	CD3DX12_RECT scissor_rects[2];

	for (int i = 0; i < 2; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)(Win32App::SIZE_.x), (FLOAT)(Win32App::SIZE_.y));
		scissor_rects[i] = CD3DX12_RECT(0, 0, (LONG)(Win32App::SIZE_.x), (LONG)(Win32App::SIZE_.y));
	}

	// �r���[�|�[�g�̐ݒ�
	command_list_->RSSetViewports(2, viewports);

	// �V�U�����O��`�̐ݒ�
	command_list_->RSSetScissorRects(2, scissor_rects);

	// �S��ʃN���A
	for (int i = 0; i < 2; i++)
	{
		command_list_->ClearRenderTargetView(rtv_h[i], clear_color_, 0, nullptr);
	}

	// �[�x�o�b�t�@�̃N���A
	command_list_->ClearDepthStencilView(dsv_h, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	// ���\�[�X�o���A�̕ύX�i�`��\->�V�F�[�_�[���\�[�X�j
	for (int i = 0; i < 2; i++)
	{
		command_list_->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(tex_buff_[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}
