#include "FbxObject3d.h"
#include <d3dcompiler.h>
#include <cassert>
#include "../3D/FbxLoader.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ComPtr<ID3D12Device> FbxObject3d::device_ = nullptr;
Camera *FbxObject3d::camera_ = nullptr;
ComPtr<ID3D12RootSignature> FbxObject3d::root_signature_;
ComPtr<ID3D12PipelineState> FbxObject3d::pipeline_state_;

void FbxObject3d::CreateGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vs_blob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> ps_blob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> error_blob; // �G���[�I�u�W�F�N�g

	assert(device_);

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
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
		L"Resources/shaders/FBXPS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
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
	D3D12_INPUT_ELEMENT_DESC input_layout[] = {
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
		{ // �e�����󂯂�{�[���ԍ��i4�j
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �{�[���̃X�L���E�F�C�g�i4�j
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC g_pipeline{};
	g_pipeline.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	g_pipeline.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());

	// �T���v���}�X�N
	g_pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	g_pipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	g_pipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blend_desc{};
	blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blend_desc.BlendEnable = true;
	blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	g_pipeline.BlendState.RenderTarget[0] = blend_desc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	g_pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	g_pipeline.InputLayout.pInputElementDescs = input_layout;
	g_pipeline.InputLayout.NumElements = _countof(input_layout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	g_pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	g_pipeline.NumRenderTargets = 1;    // �`��Ώۂ�1��
	g_pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	g_pipeline.SampleDesc.Count = 1;	// 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE desc_range_srv;
	desc_range_srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER root_params[3];
	// CBV�i���W�ϊ��s��p�j
	root_params[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	root_params[1].InitAsDescriptorTable(1, &desc_range_srv, D3D12_SHADER_VISIBILITY_ALL);
	// CBV�i�X�L�j���O�p�j
	root_params[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc;
	root_signature_desc.Init_1_0(_countof(root_params), root_params, 1, &sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> root_sig_blob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &root_sig_blob, &error_blob);
	// ���[�g�V�O�l�`���̐���
	result = device_->CreateRootSignature(0, root_sig_blob->GetBufferPointer(), root_sig_blob->GetBufferSize(), IID_PPV_ARGS(root_signature_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	g_pipeline.pRootSignature = root_signature_.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device_->CreateGraphicsPipelineState(&g_pipeline, IID_PPV_ARGS(pipeline_state_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

}

void FbxObject3d::Initialize() {
	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buffer_transform_)
	);

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buffer_skin_)
	);

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin *const_map_skin = nullptr;
	result = const_buffer_skin_->Map(0, nullptr, reinterpret_cast<void **>(&const_map_skin));
	for (int i = 0; i < MAX_BONES_; i++) {
		const_map_skin->bones[i] = XMMatrixIdentity();
	}
	const_buffer_skin_->Unmap(0, nullptr);

	// 1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frame_time_.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void FbxObject3d::Update() {
	// �A�j���[�V����
	if (is_play_) {
		// 1�t���[���i�߂�
		current_time_ += frame_time_;

		if (is_loop_ && IsAnimationEnd()) {
			// ���[�vON���Ō�܂ōĐ�������擪�ɖ߂�
			current_time_ = start_time_;
		}
	}

	XMMATRIX mat_scale, mat_rot, mat_trans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	mat_scale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	mat_rot = XMMatrixIdentity();
	mat_rot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	mat_rot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	mat_rot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	mat_trans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ���[���h�s��̍���
	mat_world_ = XMMatrixIdentity();	// �ό`�����Z�b�g
	mat_world_ *= mat_scale;			// ���[���h�s��ɃX�P�[�����O�𔽉f
	mat_world_ *= mat_rot;				// ���[���h�s��ɉ�]�𔽉f
	mat_world_ *= mat_trans;			// ���[���h�s��ɕ��s�ړ��𔽉f

	// �r���[�v���W�F�N�V�����s��
	const XMMATRIX &mat_view = camera_->GetMatView();
	const XMMATRIX &mat_proj = camera_->GetMatProjection();
	const XMMATRIX &mat_view_proj = mat_view * mat_proj;
	// ���f���̃��b�V���g�����X�t�H�[��
	//const XMMATRIX &model_transform = fbx_model_ptr_->GetModelTransform();
	// �J�������W
	const XMFLOAT3 &camera_pos = camera_->GetEye();

	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataTransform *const_map = nullptr;
	result = const_buffer_transform_->Map(0, nullptr, reinterpret_cast<void **>(&const_map));
	if (SUCCEEDED(result)) {
		/*const_map->mat_view = mat_view;
		const_map->mat_proj = mat_proj;*/
		const_map->mat_view_proj = mat_view_proj;
		//const_map->world = model_transform * mat_world_;
		const_map->world = mat_world_;
		const_map->camera_pos = camera_pos;
		const_buffer_transform_->Unmap(0, nullptr);
	}

	// �{�[���z��
	std::vector<FbxModel::Bone> &bones = fbx_model_ptr_->GetBones();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin *const_map_skin = nullptr;
	result = const_buffer_skin_->Map(0, nullptr, reinterpret_cast<void **>(&const_map_skin));
	for (int i = 0; i < bones.size(); i++) {
		// ���̎p���s��
		XMMATRIX mat_current_pose;
		// ���̎p���s����擾
		FbxAMatrix fbx_current_pose = bones[i].fbx_cluster->GetLink()->EvaluateGlobalTransform(current_time_);
		// XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&mat_current_pose, fbx_current_pose);
		// �������ăX�L�j���O�s���
		const_map_skin->bones[i] = bones[i].inverse_initial_pose * mat_current_pose;
	}
	const_buffer_skin_->Unmap(0, nullptr);
}

void FbxObject3d::Draw(ID3D12GraphicsCommandList *cmd_list) {
	// ���f���̊��蓖�Ă�������Ε`�悵�Ȃ�
	if (fbx_model_ptr_ == nullptr) {
		return;
	}

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmd_list->SetPipelineState(pipeline_state_.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmd_list->SetGraphicsRootSignature(root_signature_.Get());
	// �v���~�e�B�u�`���ݒ�
	cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list->SetGraphicsRootConstantBufferView(0, const_buffer_transform_->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list->SetGraphicsRootConstantBufferView(2, const_buffer_skin_->GetGPUVirtualAddress());

	// ���f���`��
	fbx_model_ptr_->Draw(cmd_list);
}

void FbxObject3d::PlayAnimation(int anime_enum, bool is_loop) {
	// �ʂ̃A�j���[�V�������Đ�����ꍇ
	if (anime_enum != current_anim_num_) {
		// �Đ����~���Đݒ肵�Ȃ���
		is_play_ = false;
	}

	// �Đ����Ȃ�Ȃɂ����Ȃ�
	if (is_play_) {
		return;
	}

	FbxScene *fbx_scene = fbx_model_ptr_->GetFbxScene();

	// �A�j���[�V�������擾
	FbxAnimStack *anim_stack = fbx_scene->GetSrcObject<FbxAnimStack>(anime_enum);
	// �A�j���[�V�����̖��O�擾
	const char *anim_stack_name = anim_stack->GetName();
	// �A�j���[�V�������Z�b�g
	fbx_scene->SetCurrentAnimationStack(anim_stack);
	// �A�j���[�V�����̎��ԏ��
	FbxTakeInfo *take_info = fbx_scene->GetTakeInfo(anim_stack_name);

	// �J�n���Ԏ擾
	start_time_ = take_info->mLocalTimeSpan.GetStart();
	// �I�����Ԏ擾
	end_time_ = take_info->mLocalTimeSpan.GetStop();
	// �J�n���Ԃɍ��킹��
	current_time_ = start_time_;
	// �Đ�����Ԃɂ���
	is_play_ = true;
	// ���[�v�ݒ�
	is_loop_ = is_loop;
	// ���݂̃A�j���ԍ�
	current_anim_num_ = anime_enum;
}

bool FbxObject3d::IsPlaying() {
	if (is_play_) { return true; }

	return false;
}

bool FbxObject3d::IsAnimationEnd() {
	if (current_time_ > end_time_) {
		return true;
	}

	return false;
}

// �ُ�I������ꍇ�́Anullptr�Ȃ�return����悤�ɏ���������
// ����nullptr�ɂȂ�̂����󕪂���Ȃ��I
// �̂ň��ُ�I�����o������K�v������
