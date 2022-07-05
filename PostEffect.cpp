#include "PostEffect.h"
#include "Win32App.h"
#include <d3dcompiler.h>
#include "KeyboardInput.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

const float PostEffect::clear_color_[4] = { 0.25f, 0.5f, 0.1f, 0.0f };

PostEffect::PostEffect()
	: Sprite(
		100,
		{ 0, 0 },
		{ 500.0f, 500.0f },
		{ 1, 1, 1, 1 },
		{ 0.0f, 0.0f },
		false,
		false
	) {
}

void PostEffect::CreateGraphicsPipelineState() {

	HRESULT result;

	ComPtr<ID3DBlob> vsBlob;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {

		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {

		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {

		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // ��ɏ㏑�����[��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	CD3DX12_DESCRIPTOR_RANGE desc_range_srv_1;
	desc_range_srv_1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &desc_range_srv_1, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 ���W�X�^

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&root_signature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = root_signature_.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_state_));
	assert(SUCCEEDED(result));
}

void PostEffect::Initialize() {

	// �p�C�v���C���̐���
	CreateGraphicsPipelineState();

	HRESULT result;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	assert(SUCCEEDED(result));

	// ���_�f�[�^
	VertexPosUv vertices[vertNum] = {

		{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},	// ����
		{{-1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},	// ����
		{{+1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},	// �E��
		{{+1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},	// �E��
	};

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vert_map = nullptr;
	result = vertBuff->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {

		memcpy(vert_map, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	assert(SUCCEEDED(result));

	// �e�N�X�`�����\�[�X����
	CD3DX12_RESOURCE_DESC tex_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		Win32App::window_width_,
		static_cast<UINT>(Win32App::window_height_),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	// �e�N�X�`���o�b�t�@�̐���
	for (int i = 0; i < 2; i++) {

		result = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&tex_res_desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clear_color_),
			IID_PPV_ARGS(&tex_buff_[i]));
	}

	assert(SUCCEEDED(result));

	// �e�N�X�`����ԃN���A
	//{
	//	// ��f��(1280 x 720 = 9216000 pix)
	//	const UINT pixel_count = Win32App::window_width_ * Win32App::window_height_;

	//	// �摜1�s���̃f�[�^�T�C�Y
	//	const UINT row_pitch = sizeof(UINT) * Win32App::window_width_;

	//	// �摜�S�̂̃f�[�^�T�C�Y
	//	const UINT depth_pitch = row_pitch * Win32App::window_height_;

	//	// �摜�C���[�W
	//	UINT *img = new UINT[pixel_count];
	//	for (int i = 0; i < pixel_count; i++) { img[i] = 0xff0000ff; }

	//	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//	result = tex_buff_->WriteToSubresource(0, nullptr, img, row_pitch, depth_pitch);

	//	assert(SUCCEEDED(result));

	//	delete[] img;
	//}

	// SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srv_desc_heap_desc = {};
	srv_desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srv_desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srv_desc_heap_desc.NumDescriptors = 2;

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srv_desc_heap_desc, IID_PPV_ARGS(&desc_heap_SRV_));
	assert(SUCCEEDED(result));

	// SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;

	// �f�X�N���v�^�q�[�v��SRV�쐬
	for (int i = 0; i < 2; i++) {

		device->CreateShaderResourceView(tex_buff_[i].Get(),
			&srv_desc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				desc_heap_SRV_->GetCPUDescriptorHandleForHeapStart(), i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}

	// RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtv_desc_heap_desc{};
	rtv_desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_desc_heap_desc.NumDescriptors = 2;

	// RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtv_desc_heap_desc, IID_PPV_ARGS(&desc_heap_RTV_));

	assert(SUCCEEDED(result));

	for (int i = 0; i < 2; i++) {

		// �f�X�N���v�^�q�[�v��RTV�쐬
		device->CreateRenderTargetView(tex_buff_[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				desc_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
		);
	}
	// �f�v�X�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depth_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Win32App::window_width_,
		Win32App::window_height_,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	// �[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depth_res_desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depth_buff_)
	);

	assert(SUCCEEDED(result));

	// DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc{};
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc_heap_desc.NumDescriptors = 1;

	// DSV�p�f�X�N���v�^�q�[�v���쐬
	result = device->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&desc_heap_DSV_));

	assert(SUCCEEDED(result));

	// �f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
	dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depth_buff_.Get(),
		&dsv_desc,
		desc_heap_DSV_->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Draw() {

	/*if (KeyboardInput::TriggerKey(DIK_1)) {

		static int tex = 0;

		tex = (tex + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
		srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(tex_buff_[tex].Get(),
			&srv_desc,
			desc_heap_SRV_->GetCPUDescriptorHandleForHeapStart()
		);
	}*/

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = XMMatrixIdentity();	// �s��̍���	
		this->constBuff->Unmap(0, nullptr);
	}

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmd_list_->SetPipelineState(pipeline_state_.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmd_list_->SetGraphicsRootSignature(root_signature_.Get());
	// �v���~�e�B�u�`���ݒ�
	cmd_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�o�b�t�@�̐ݒ�
	cmd_list_->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap *ppHeaps[] = { desc_heap_SRV_.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmd_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list_->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmd_list_->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_SRV_->GetGPUDescriptorHandleForHeapStart(), 0,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	));
	cmd_list_->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_SRV_->GetGPUDescriptorHandleForHeapStart(), 1,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	));
	//cmd_list_->SetGraphicsRootDescriptorTable(1, desc_heap_SRV_->GetGPUDescriptorHandleForHeapStart());
	// �`��R�}���h
	cmd_list_->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene() {

	// ���\�[�X�o���A��ύX�i�V�F�[�_�[���\�[�X->�`��\�j

	for (int i = 0; i < 2; i++) {

		cmd_list_->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(tex_buff_[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET)
		);
	}
	// �����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_h[2];
	for (int i = 0; i < 2; i++) {

		rtv_h[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			desc_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	// �f�v�X�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsv_h = desc_heap_DSV_->GetCPUDescriptorHandleForHeapStart();

	// �����_�[�^�[�Q�b�g���Z�b�g
	cmd_list_->OMSetRenderTargets(2, rtv_h, false, &dsv_h);

	CD3DX12_VIEWPORT viewports[2];
	CD3DX12_RECT scissor_rects[2];

	for (int i = 0; i < 2; i++) {

		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, Win32App::window_width_, Win32App::window_height_);
		scissor_rects[i] = CD3DX12_RECT(0, 0, Win32App::window_width_, Win32App::window_height_);
	}

	// �r���[�|�[�g�̐ݒ�
	cmd_list_->RSSetViewports(2, viewports);

	// �V�U�����O��`�̐ݒ�
	cmd_list_->RSSetScissorRects(2, scissor_rects);

	// �S��ʃN���A
	for (int i = 0; i < 2; i++) {

		cmd_list_->ClearRenderTargetView(rtv_h[i], clear_color_, 0, nullptr);
	}

	// �[�x�o�b�t�@�̃N���A
	cmd_list_->ClearDepthStencilView(dsv_h, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene() {

	// ���\�[�X�o���A�̕ύX�i�`��\->�V�F�[�_�[���\�[�X�j
	for (int i = 0; i < 2; i++) {

		cmd_list_->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(tex_buff_[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}
