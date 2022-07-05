#include "ExecuteIndirectDemoScene.h"
#include "DirectXBase.h"
#include <d3dcompiler.h>
#include "KeyboardInput.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

const UINT ExecuteIndirectDemoScene::command_size_per_frame_ = all_particle_num_ * sizeof(IndirectCommand);

ExecuteIndirectDemoScene::ExecuteIndirectDemoScene() {

	const_buffer_data_.resize(all_particle_num_);
	matrix_const_buffer_data_.resize(all_particle_num_);
	material_const_buffer_data_.resize(all_particle_num_);

	// �J�����̐���
	camera_ = make_unique<Camera>();

	// ���f���f�[�^�̐���
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");

	object_ = std::make_unique<Object3d>();
	object_->SetModel(model_.get());
	object_->Initialize();
	//IndirectObject3D::SetCamera(camera_.get());

	// �p�[�e�B�N���f�[�^�̐���
	emitter_1_ = make_unique<Emitter>();
	emitter_2_ = make_unique<Emitter>();
}

ExecuteIndirectDemoScene::~ExecuteIndirectDemoScene() {
}

void ExecuteIndirectDemoScene::CreateDescriptorHeap() {

	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v���쐬
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
	desc_heap_desc.NumDescriptors = 3 * frame_count_;
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	device_->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&descriptor_heap_));

	assert(SUCCEEDED(result));

	// �f�X�N���v�^�T�C�Y���擾
	descriptor_heap_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ExecuteIndirectDemoScene::CreateRootSignature() {

	HRESULT result;

	D3D12_FEATURE_DATA_ROOT_SIGNATURE feature_data = {};

	feature_data.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	if (FAILED(device_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &feature_data, sizeof(feature_data)))) {
		feature_data.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE desc_range_SRV;
	desc_range_SRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER root_parameters[2];
	root_parameters[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	root_parameters[1].InitAsDescriptorTable(1, &desc_range_SRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc;
	//root_signature_desc.Init_1_1(_countof(root_parameters), root_parameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	root_signature_desc.Init_1_0(_countof(root_parameters), root_parameters, 1, &sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> root_signature_blob;
	ComPtr<ID3DBlob> error_blob;

	// �o�[�W��������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&root_signature_desc, feature_data.HighestVersion, &root_signature_blob, &error_blob);

	// ���[�g�V�O�l�`���̐���
	result = device_->CreateRootSignature(0,
		root_signature_blob->GetBufferPointer(),
		root_signature_blob->GetBufferSize(),
		IID_PPV_ARGS(&root_signature_));

	// �ȉ��ŃR���s���[�g���[�g�V�O�l�`���𐶐�����K�v����
}

void ExecuteIndirectDemoScene::CreatePipelineState() {

	//	HRESULT result;
	//
	//	ComPtr<ID3DBlob> vs_blob;
	//	ComPtr<ID3DBlob> ps_blob;
	//	ComPtr<ID3DBlob> error_blob;
	//
	//#if defined(_DEBUG)
	//	// �O���t�B�b�N�X�f�o�b�O�c�[���ł��ǂ��V�F�[�_�[�f�o�b�O���\�ɂ���
	//	UINT compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	//#else
	//	UINT compile_flags = 0;
	//#endif
	//
	//	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	//	result = D3DCompileFromFile(
	//		L"Resources/shaders/ParticleVS.hlsl",
	//		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
	//		"main", "vs_5_0", compile_flags, 0, &vs_blob, &error_blob);
	//
	//	if (FAILED(result)) {
	//
	//		// errorBlob����G���[���e��string�^�ɃR�s�[
	//		std::string errstr;
	//		errstr.resize(error_blob->GetBufferSize());
	//
	//		std::copy_n((char *)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
	//		errstr += "\n";
	//
	//		// �G���[���e���o�̓E�B���h�E�ɕ\��
	//		OutputDebugStringA(errstr.c_str());
	//		exit(1);
	//	}
	//
	//	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	//	result = D3DCompileFromFile(
	//		L"Resources/shaders/ParticlePS.hlsl",
	//		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
	//		"main", "ps_5_0", compile_flags, 0, &ps_blob, &error_blob);
	//
	//	if (FAILED(result)) {
	//
	//		// errorBlob����G���[���e��string�^�ɃR�s�[
	//		std::string errstr;
	//		errstr.resize(error_blob->GetBufferSize());
	//
	//		std::copy_n((char *)error_blob->GetBufferPointer(), error_blob->GetBufferSize(), errstr.begin());
	//		errstr += "\n";
	//
	//		// �G���[���e���o�̓E�B���h�E�ɕ\��
	//		OutputDebugStringA(errstr.c_str());
	//		exit(1);
	//	}
	//
	//	// ���_���C�A�E�g
	//	D3D12_INPUT_ELEMENT_DESC input_layout[] = {
	//
	//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//	};
	//
	//	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	//	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	//	gpipeline.InputLayout = { input_layout , _countof(input_layout) };
	//	gpipeline.pRootSignature = root_signature_.Get();
	//	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	//	gpipeline.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());
	//	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//	gpipeline.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//	gpipeline.SampleMask = UINT_MAX;
	//	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//	gpipeline.NumRenderTargets = 1;
	//	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//	gpipeline.SampleDesc.Count = 1;
	//
	//	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_state_));

		// �ȉ��ŃR���s���[�g�p�C�v���C���X�e�[�g�𐶐�����K�v����

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vs_blob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> ps_blob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> error_blob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vs_blob, &error_blob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(),
			error_blob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&ps_blob, &error_blob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(),
			error_blob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
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
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;	// �`��Ώۂ�2��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
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
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &error_blob);
	// ���[�g�V�O�l�`���̐���
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&root_signature_));

	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = root_signature_.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_state_));

	assert(SUCCEEDED(result));
}

void ExecuteIndirectDemoScene::CreateGraphicsPipeline() {
}

void ExecuteIndirectDemoScene::CreateConstantBuffer() {

	HRESULT result;

	const UINT const_buffer_data_size = resource_count_ * sizeof(ConstBufferData);

	/*result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(const_buffer_data_size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buffer_));*/

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrixConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_));

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Model::MaterialConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_const_buffer_));

	for (UINT i = 0; i < all_particle_num_; i++) {

		material_const_buffer_data_[i].ambient = model_->GetMaterialData().ambient;
		material_const_buffer_data_[i].diffuse = model_->GetMaterialData().diffuse;
		material_const_buffer_data_[i].specular = model_->GetMaterialData().specular;
		material_const_buffer_data_[i].alpha = model_->GetMaterialData().alpha;

		matrix_const_buffer_data_[i].mat = XMMatrixIdentity();	// �����

		const_buffer_data_[i].position_ = { 0, 0, 0 };
		const_buffer_data_[i].velocity_ = { 0, 0, 0 };
		const_buffer_data_[i].color_ = { 0, 0, 0 };
		//const_buffer_data_[i].matrix_ = XMMatrixIdentity();
	}

	Model::MaterialConstBufferData *material_const_map = nullptr;
	result = material_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(material_const_map));
	//memcpy(material_const_map, &material_const_buffer_data_[0], all_particle_num_ * sizeof(Model::MaterialConstBufferData));
	for (UINT i = 0; i < material_const_buffer_data_.size(); i++) {

		material_const_map = &material_const_buffer_data_[i];
	}
	material_const_buffer_->Unmap(0, nullptr);

	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(matrix_const_map));
	for (UINT i = 0; i < matrix_const_buffer_data_.size(); i++) {

		matrix_const_map = &matrix_const_buffer_data_[i];
	}
	matrix_const_buffer_->Unmap(0, nullptr);
}

void ExecuteIndirectDemoScene::CreateCommandSignature() {

	HRESULT result = S_FALSE;

	D3D12_INDIRECT_ARGUMENT_DESC argument_desc[2]{};
	argument_desc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
	argument_desc[0].ConstantBufferView.RootParameterIndex = 0;
	argument_desc[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	D3D12_COMMAND_SIGNATURE_DESC command_signature_desc = {};
	command_signature_desc.pArgumentDescs = argument_desc;
	command_signature_desc.NumArgumentDescs = _countof(argument_desc);
	command_signature_desc.ByteStride = sizeof(IndirectCommand);

	result = device_->CreateCommandSignature(&command_signature_desc, root_signature_.Get(), IID_PPV_ARGS(&command_signature_));
}

void ExecuteIndirectDemoScene::CreateCommandBuffer() {

	// �����T���v��������p----------------------------------------------------------------
	// ����: ComPtr �� CPU �I�u�W�F�N�g�ł����A�����̃��\�[�X�́A
	// �������Q�Ƃ���R�}���h���X�g�� GPU ��Ŏ��s���I������܂ŁA
	// �����̃��\�[�X�̓X�R�[�v���ɂ���K�v������܂��B
	// ���̃��\�b�h�̍Ō�� GPU ���t���b�V�����A���\�[�X�������ɔj�󂳂�Ȃ��悤�ɂ��܂��B
	// ���������ɔj������Ȃ��悤�ɂ��邽�߁A���̃��\�b�h�̍Ō�� GPU ���t���b�V�����܂��B
	ComPtr<ID3D12Resource> vertex_buffer_upload;
	ComPtr<ID3D12Resource> command_buffer_upload;

	HRESULT result;

	std::vector<IndirectCommand> commands;
	commands.resize(resource_count_);
	const UINT command_buffer_size = command_size_per_frame_ * frame_count_;

	D3D12_RESOURCE_DESC command_buffer_desc = CD3DX12_RESOURCE_DESC::Buffer(command_buffer_size);

	// �R�}���h�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&command_buffer_desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&command_buffer_)
	);

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(command_buffer_size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&command_buffer_upload)
	);

	//D3D12_GPU_VIRTUAL_ADDRESS gpu_address = const_buffer_->GetGPUVirtualAddress();
	D3D12_GPU_VIRTUAL_ADDRESS matrix_gpu_address = matrix_const_buffer_->GetGPUVirtualAddress();
	D3D12_GPU_VIRTUAL_ADDRESS material_gpu_address = material_const_buffer_->GetGPUVirtualAddress();
	UINT command_index = 0;
	UINT vertex_count = (UINT)(model_->GetVertices().size());

	/*IndirectCommand *command_map = nullptr;
	command_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&commands));*/
	//command_buffer_->Map(0, nullptr, reinterpret_cast<void **>(command_map));

	for (UINT frame = 0; frame < frame_count_; frame++) {
		for (UINT n = 0; n < all_particle_num_; n++) {

			//commands[command_index].cbv_ = gpu_address;
			commands[command_index].matrix_cbv_ = matrix_gpu_address;
			commands[command_index].material_cbv_ = material_gpu_address;

			// ������DrawIndexedInstanced�̈����ƑS������
			commands[command_index].draw_arguments_.IndexCountPerInstance = vertex_count;
			commands[command_index].draw_arguments_.InstanceCount = 1;
			commands[command_index].draw_arguments_.StartIndexLocation = 0;
			commands[command_index].draw_arguments_.BaseVertexLocation = 0;
			commands[command_index].draw_arguments_.StartInstanceLocation = 0;

			command_index++;
			//gpu_address += sizeof(ConstBufferData);
			matrix_gpu_address += sizeof(MatrixConstBufferData);
			material_gpu_address += sizeof(Model::MaterialConstBufferData);
		}
	}
	//command_buffer_->Unmap(0, nullptr);

	// �f�[�^�𒆊ԃA�b�v���[�h�q�[�v�ɃR�s�[���A���̌�A�A�b�v���[�h�q�[�v����R�}���h�o�b�t�@�ւ̃R�s�[���X�P�W���[�����܂��B
	// �A�b�v���[�h�q�[�v����R�}���h�o�b�t�@�ɃR�s�[����B
	D3D12_SUBRESOURCE_DATA command_data = {};
	command_data.pData = reinterpret_cast<UINT8 *>(&commands[0]);
	command_data.RowPitch = command_buffer_size;
	command_data.SlicePitch = command_data.RowPitch;

	UpdateSubresources<1>(command_list_.Get(), command_buffer_.Get(), command_buffer_upload.Get(), 0, 0, 1, &command_data);
	//command_list_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(command_buffer_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	// �R�}���h�o�b�t�@�p��SRV�𐶐�
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.Buffer.NumElements = all_particle_num_;
	srv_desc.Buffer.StructureByteStride = sizeof(IndirectCommand);
	srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	CD3DX12_CPU_DESCRIPTOR_HANDLE commands_handle(descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), 1, descriptor_heap_size_);

	// offset������Ă邩�킩��� �Ȃ�Ȃ�
	for (UINT frame = 0; frame < frame_count_; frame++) {

		srv_desc.Buffer.FirstElement = frame * all_particle_num_;
		device_->CreateShaderResourceView(command_buffer_.Get(), &srv_desc, commands_handle);
		commands_handle.Offset(3, descriptor_heap_size_);
	}
}

void ExecuteIndirectDemoScene::TransferConstantBafferData() {

	HRESULT result;
	XMMATRIX mat_scale, mat_rot, mat_trans, mat_world;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	mat_scale = XMMatrixScaling(object_->GetScale().x, object_->GetScale().y, object_->GetScale().z);
	mat_rot = XMMatrixIdentity();
	mat_rot *= XMMatrixRotationZ(XMConvertToRadians(object_->GetRotation().z));
	mat_rot *= XMMatrixRotationX(XMConvertToRadians(object_->GetRotation().x));
	mat_rot *= XMMatrixRotationY(XMConvertToRadians(object_->GetRotation().y));
	mat_trans = XMMatrixTranslation(object_->GetPosition().x, object_->GetPosition().y, object_->GetPosition().z);

	// ���[���h�s��̍���
	mat_world = XMMatrixIdentity();		// �ό`�����Z�b�g
	mat_world *= mat_scale;				// ���[���h�s��ɃX�P�[�����O�𔽉f
	mat_world *= mat_rot;				// ���[���h�s��ɉ�]�𔽉f
	mat_world *= mat_trans;				// ���[���h�s��ɕ��s�ړ��𔽉f

	// �萔�o�b�t�@�փf�[�^�]��
	Model::MaterialConstBufferData *material_const_map = nullptr;
	result = material_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&material_const_map));
	material_const_map->ambient = model_->GetMaterialData().ambient;
	material_const_map->diffuse = model_->GetMaterialData().diffuse;
	material_const_map->specular = model_->GetMaterialData().specular;
	material_const_map->alpha = model_->GetMaterialData().alpha;
	material_const_buffer_->Unmap(0, nullptr);

	const XMMATRIX &mat_view_ = camera_->GetMatView();
	const XMMATRIX &mat_projection = camera_->GetMatProjection();

	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&matrix_const_map));
	matrix_const_map->mat = mat_world * mat_view_ * mat_projection;	// �s��̍���
	matrix_const_buffer_->Unmap(0, nullptr);
}

void ExecuteIndirectDemoScene::Initialize() {

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 0, 0 });
	Object3d::SetCamera(camera_.get());
	//IndirectObject3d::SetCamera(camera_.get());

	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	command_list_ = DirectXBase::GetInstance()->GetCmdList().Get();

	//object_->Initialize();

	CreateDescriptorHeap();
	//CreateRootSignature();
	CreatePipelineState();
	CreateConstantBuffer();
	CreateCommandSignature();
	CreateCommandBuffer();

	Emitter::StaticInitialize();

	// �p�[�e�B�N���̏�����
	p_.position_ = { 10.0f, 0.0f, -50.0f };
	p_.velocity_ = { 0.0f, 0.f, -1.0f };
	p_.accel_ = { 0, 0.001f, 0 };
	p_.life_ = 100;
	p_.s_scale_ = 1.0f;
	range_ = { 0.0f, 0.0f, 0.0f };
	generate_num_ = 1;

	p_2_.position_ = { -10.0f, 0.0f, -50.0f };
	p_2_.velocity_ = { 0.0f, 0.f, -1.0f };
	p_2_.accel_ = { 0, 0.001f, 0 };
	p_2_.life_ = 100;
	p_2_.s_scale_ = 1.0f;

	TransferConstantBafferData();
}

void ExecuteIndirectDemoScene::Finalize() {
}

void ExecuteIndirectDemoScene::Update() {

	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F)) {

		if (KeyboardInput::PushKey(DIK_W)) { camera_->MoveCameraTrack({ 0.0f,+1.0f,0.0f }); } else if (KeyboardInput::PushKey(DIK_S)) { camera_->MoveCameraTrack({ 0.0f,-1.0f,0.0f }); }

		if (KeyboardInput::PushKey(DIK_D)) { camera_->MoveCameraTrack({ +1.0f, 0.0f, 0.0f }); } else if (KeyboardInput::PushKey(DIK_A)) { camera_->MoveCameraTrack({ -1.0f,0.0f,0.0f }); }

		if (KeyboardInput::PushKey(DIK_R)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, +1.0f }); } else if (KeyboardInput::PushKey(DIK_F)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, -1.0f }); }
	}

	if (KeyboardInput::PushKey(DIK_SPACE)) {

		p_.position_ = { 10.0f, 0.0f, -50.0f };
		p_2_.position_ = { -10.0f, 0.0f, -50.0f };
	}

	camera_->Update();

	//TransferConstantBafferData();

	// �p�[�e�B�N���𐶐�
	/*p_.position_.z += 1.0f;
	p_2_.position_.z += 1.0f;
	emitter_1_->GenerateParticle(p_, range_, generate_num_);
	emitter_2_->GenerateParticle(p_2_, range_, generate_num_);*/
}

void ExecuteIndirectDemoScene::Draw() {

	// �R�}���h�V�O�l�`���ɓo�^��������count_��`�悷��H
	command_list_->ExecuteIndirect(
		command_signature_.Get(),
		all_particle_num_,
		command_buffer_.Get(),
		static_cast<UINT64>(command_size_per_frame_) * frame_index_,
		nullptr,
		0);

	// �`��
	/*emitter_1_->Draw();
	emitter_2_->Draw();*/
}

void ExecuteIndirectDemoScene::DebugDraw() {
}
