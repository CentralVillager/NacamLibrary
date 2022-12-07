#include "NcmPlatePoly.h"
#include <DirectXTex.h>
#include "../../App/Utility/NcmUtil.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace NcmUtill;

ID3D12Device *NcmPlatePoly::device_ = nullptr;
UINT NcmPlatePoly::desc_incre_size_ = 0;
ID3D12GraphicsCommandList *NcmPlatePoly::cmd_list_ = nullptr;
ComPtr<ID3D12DescriptorHeap> NcmPlatePoly::desc_heap_;
ComPtr<ID3D12Resource> NcmPlatePoly::vert_buff_;
ComPtr<ID3D12Resource> NcmPlatePoly::tex_buff_;
CD3DX12_CPU_DESCRIPTOR_HANDLE NcmPlatePoly::cpu_desc_handle_srv_;
CD3DX12_GPU_DESCRIPTOR_HANDLE NcmPlatePoly::gpu_desc_handle_srv_;
D3D12_VERTEX_BUFFER_VIEW NcmPlatePoly::vb_view_{};
NcmPlatePoly::VertexPos NcmPlatePoly::vertices_[VERTEX_COUNT_];
Camera *NcmPlatePoly::cam_ptr_ = nullptr;

NcmPlatePoly::NcmPlatePoly() :
	const_buff_(),
	draw_vert_size_()
{}

NcmPlatePoly::~NcmPlatePoly()
{}

bool NcmPlatePoly::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list)
{
	// nullptr�`�F�b�N
	assert(device);

	device_ = device;
	cmd_list_ = cmd_list;

	// �f�X�N���v�^�q�[�v�̏�����
	InitializeDescriptorHeap();

	// �e�N�X�`���ǂݍ���
	LoadTexture();

	// ���f������
	CreateModel();

	return true;
}

bool NcmPlatePoly::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	desc_heap_desc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device_->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&desc_heap_));//����
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �f�X�N���v�^�T�C�Y���擾
	desc_incre_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool NcmPlatePoly::LoadTexture()
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratch_img{};

	result = LoadFromWICFile(
		L"Resources/Textures/effect1.png", WIC_FLAGS_NONE,
		&metadata, scratch_img);
	assert(SUCCEEDED(result));

	const Image *img = scratch_img.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC tex_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&tex_res_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&tex_buff_));
	assert(SUCCEEDED(result));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = tex_buff_->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);
	assert(SUCCEEDED(result));

	// �V�F�[�_���\�[�X�r���[�쐬
	cpu_desc_handle_srv_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(desc_heap_->GetCPUDescriptorHandleForHeapStart(), 0, desc_incre_size_);
	gpu_desc_handle_srv_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_->GetGPUDescriptorHandleForHeapStart(), 0, desc_incre_size_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC res_desc = tex_buff_->GetDesc();

	srv_desc.Format = res_desc.Format;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srv_desc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView(tex_buff_.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srv_desc, //�e�N�X�`���ݒ���
		cpu_desc_handle_srv_
	);

	return true;
}

void NcmPlatePoly::CreateModel()
{
	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices_)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vert_buff_));
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPos *vert_map = nullptr;
	result = vert_buff_->Map(0, nullptr, (void **)&vert_map);
	if (SUCCEEDED(result))
	{
		memcpy(vert_map, vertices_, sizeof(vertices_));
		vert_buff_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vb_view_.BufferLocation = vert_buff_->GetGPUVirtualAddress();
	vb_view_.SizeInBytes = sizeof(vertices_);
	vb_view_.StrideInBytes = sizeof(vertices_[0]);
}

void NcmPlatePoly::Initialize()
{
	// nullptr�`�F�b�N
	assert(device_);

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buff_));
}

void NcmPlatePoly::UpdateVertBuffer(std::forward_list<NcmParticleCommonArgs> &args)
{
	// ���_�o�b�t�@�փf�[�^�]��
	VertexPos *vert_map = nullptr;
	HRESULT result = vert_buff_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));

	if (SUCCEEDED(result))
	{
		// �p�[�e�B�N���̏���1�����f
		for (auto itr = args.begin(); itr != args.end(); itr++)
		{
			// ���W
			vert_map->pos = itr->pos;
			// �X�P�[��
			vert_map->scale = itr->scale;
			// ���̍��W��
			vert_map++;
		}
	}
	vert_buff_->Unmap(0, nullptr);

	// �v�f���i���_���j���擾
	draw_vert_size_ = (UINT)(std::distance(args.begin(), args.end()));
}

void NcmPlatePoly::UpdateConstBuffer()
{
	// �萔�o�b�t�@�փf�[�^��]��
	ConstBufferData *const_map = nullptr;
	HRESULT result = const_buff_->Map(0, nullptr, (void **)&const_map);
	const_map->mat = cam_ptr_->GetMatView() * cam_ptr_->GetMatProjection();	// �s��̍���
	const_map->mat_billboard = cam_ptr_->GetMatBillboard();
	const_buff_->Unmap(0, nullptr);
}

void NcmPlatePoly::Draw()
{
	// nullptr�`�F�b�N
	assert(device_);
	assert(cmd_list_);

	// ���_�o�b�t�@�̐ݒ�
	cmd_list_->IASetVertexBuffers(0, 1, &vb_view_);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap *pp_heaps[] = { desc_heap_.Get() };
	cmd_list_->SetDescriptorHeaps(_countof(pp_heaps), pp_heaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list_->SetGraphicsRootConstantBufferView(0, const_buff_->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmd_list_->SetGraphicsRootDescriptorTable(1, gpu_desc_handle_srv_);
	// �`��R�}���h
	cmd_list_->DrawInstanced(draw_vert_size_, 1, 0, 0);
}
