#include "PrimitiveObject.h"
#include <d3dcompiler.h>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
using namespace DirectX;

ComPtr<ID3D12Device> PrimitiveObject::device_;
UINT PrimitiveObject::descriptor_handle_increment_size_;
ComPtr<ID3D12GraphicsCommandList> PrimitiveObject::cmd_list_;
ComPtr<ID3D12RootSignature> PrimitiveObject::root_signature_;
ComPtr<ID3D12PipelineState> PrimitiveObject::pipeline_state_;
ComPtr<ID3D12DescriptorHeap> PrimitiveObject::desc_heap_;
Camera *PrimitiveObject::cam_ptr_;

void PrimitiveObject::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList) {

	device_ = device;
	cmd_list_ = cmdList;

	InitializeDescriptorHeap();

	InitializeGraphicsPipeline();
}

bool PrimitiveObject::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&desc_heap_));//����
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptor_handle_increment_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool PrimitiveObject::InitializeGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PrimitiveVS.hlsl",	// �V�F�[�_�t�@�C����
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
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PrimitivePS.hlsl",	// �V�F�[�_�t�@�C����
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
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
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
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

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
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&root_signature_));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = root_signature_.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_state_));

	if (FAILED(result)) {
		return result;
	}

	return true;
}

void PrimitiveObject::PreDraw() {

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmd_list_->SetPipelineState(pipeline_state_.Get());

	// ���[�g�V�O�l�`���̐ݒ�
	cmd_list_->SetGraphicsRootSignature(root_signature_.Get());

	// �v���~�e�B�u�`���ݒ�
	cmd_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void PrimitiveObject::Initialize(XMFLOAT3 start, XMFLOAT3 end) {
	// nullptr�`�F�b�N
	assert(device_);

	HRESULT result;
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrixConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_)
	);

	// ���_�f�[�^�𔽉f----------------
	VertexData vertex{};
	vertex.pos = start;
	vertices_.emplace_back(vertex);
	indices_.emplace_back(0);

	vertex.pos = end;
	vertices_.emplace_back(vertex);
	indices_.emplace_back(1);
	//----------------------------------

	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices_.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	// ���_�o�b�t�@����
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �C���f�b�N�X�o�b�t�@����
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buffer_));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexData *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vert_map);
		vertex_buffer_->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = index_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&indexMap));
	if (SUCCEEDED(result)) {
		// �S�C���f�b�N�X�ɑ΂���
		std::copy(indices_.begin(), indices_.end(), indexMap);
		index_buffer_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeVB;
	vertex_buffer_view_.StrideInBytes = sizeof(vertices_[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
	index_buffer_view_.Format = DXGI_FORMAT_R16_UINT;
	index_buffer_view_.SizeInBytes = sizeIB;
}

void PrimitiveObject::Finalize() {
}

void PrimitiveObject::Update() {
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ���[���h�s��̍���
	mat_world_ = XMMatrixIdentity();	// �ό`�����Z�b�g
	mat_world_ *= matScale;				// ���[���h�s��ɃX�P�[�����O�𔽉f
	mat_world_ *= matRot;				// ���[���h�s��ɉ�]�𔽉f
	mat_world_ *= matTrans;				// ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (parent_ != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		mat_world_ *= parent_->mat_world_;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	const XMMATRIX &mat_view_ = cam_ptr_->GetMatView();
	const XMMATRIX &mat_projection = cam_ptr_->GetMatProjection();

	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&matrix_const_map));
	matrix_const_map->mat = mat_world_ * mat_view_ * mat_projection;	// �s��̍���
	matrix_const_buffer_->Unmap(0, nullptr);

	/*vertices_[0].pos.z += 0.001f;
	vertices_[1].pos.z += 0.001f;*/

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexData *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vert_map);
		vertex_buffer_->Unmap(0, nullptr);
	}
}

void PrimitiveObject::Draw() {
}

void PrimitiveObject::DrawBox(XMFLOAT2 left_upper, XMFLOAT2 right_downer, int color) {
}

void PrimitiveObject::DrawLine() {
	// nullptr�`�F�b�N
	assert(device_);
	assert(PrimitiveObject::cmd_list_);

	// ���_�o�b�t�@�̐ݒ�
	cmd_list_->IASetVertexBuffers(0, 1, &vertex_buffer_view_);

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmd_list_->IASetIndexBuffer(&index_buffer_view_);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap *ppHeaps[] = { desc_heap_.Get() };
	cmd_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list_->SetGraphicsRootConstantBufferView(0, matrix_const_buffer_->GetGPUVirtualAddress());

	// �`��R�}���h
	cmd_list_->DrawInstanced(2, 1, 0, 0);
}
