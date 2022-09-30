#include "Sprite.h"
#include <cassert>
#include <d3dx12.h>
#include <DirectXTex.h>
#include "../Win32App/Win32App.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> Sprite::device_ = nullptr;
UINT Sprite::desc_handle_incre_size_;
ComPtr<ID3D12GraphicsCommandList> Sprite::cmd_list_ = nullptr;
XMMATRIX Sprite::mat_projection_;
ComPtr<ID3D12DescriptorHeap> Sprite::desc_heap_;
ComPtr<ID3D12Resource> Sprite::tex_buff_[srv_count_];
UINT Sprite::handle_counter_ = 0;
std::vector<Sprite::DrawData> Sprite::sprite_hub_;

void Sprite::TermSprite()
{
	sprite_hub_.clear();
}

void Sprite::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list)
{
	HRESULT result;

	device_ = device;
	cmd_list_ = cmd_list;

	desc_handle_incre_size_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// �ˉe�s��v�Z
	mat_projection_ = XMMatrixOrthographicOffCenterLH(
		0.0f,
		float(Win32App::SIZE_.x), float(Win32App::SIZE_.y),
		0.0f, 0.0f, 1.0f);

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = srv_count_;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&desc_heap_));//����

	assert(SUCCEEDED(result));
}

int Sprite::LoadTex(const wchar_t *filename)
{
	handle_counter_++;
	int32_t count_for_array = handle_counter_ - 1;

	// nullptr�`�F�b�N
	assert(device_);

	HRESULT result;

	TexMetadata meta_data{};
	ScratchImage scratch_img{};

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		filename, WIC_FLAGS_NONE,
		&meta_data, scratch_img);
	assert(SUCCEEDED(result));

	const Image *img = scratch_img.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC tex_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		meta_data.format,
		meta_data.width,
		(UINT)meta_data.height,
		(UINT16)meta_data.arraySize,
		(UINT16)meta_data.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&tex_res_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&tex_buff_[count_for_array]));
	assert(SUCCEEDED(result));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = tex_buff_[count_for_array]->WriteToSubresource(
		0,
		nullptr,				// �S�̈�փR�s�[
		img->pixels,			// ���f�[�^�A�h���X
		(UINT)img->rowPitch,	// 1���C���T�C�Y
		(UINT)img->slicePitch	// 1���T�C�Y
	);
	assert(SUCCEEDED(result));

	// �V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = tex_buff_[count_for_array]->GetDesc();

	srv_desc.Format = resDesc.Format;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srv_desc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView(tex_buff_[count_for_array].Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srv_desc, //�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(desc_heap_->GetCPUDescriptorHandleForHeapStart(), count_for_array, desc_handle_incre_size_)
	);

	GenerateDrawData(count_for_array);

	return count_for_array;
}

void Sprite::DrawTex(int handle)
{
	// ���[���h�s��̍X�V
	sprite_hub_[handle].mat_world_ = XMMatrixIdentity();
	sprite_hub_[handle].mat_world_ *= XMMatrixRotationZ(XMConvertToRadians(sprite_hub_[handle].rotation_));
	sprite_hub_[handle].mat_world_ *= XMMatrixTranslation(sprite_hub_[handle].position_.x, sprite_hub_[handle].position_.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *const_map = nullptr;
	HRESULT result = sprite_hub_[handle].const_buffer_->Map(0, nullptr, (void **)&const_map);
	if (SUCCEEDED(result))
	{
		const_map->color = sprite_hub_[handle].color_;
		const_map->mat = sprite_hub_[handle].mat_world_ * mat_projection_;
		sprite_hub_[handle].const_buffer_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�̐ݒ�
	cmd_list_->IASetVertexBuffers(0, 1, &sprite_hub_[handle].vb_view_);
	ID3D12DescriptorHeap *ppHeaps[] = { desc_heap_.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmd_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list_->SetGraphicsRootConstantBufferView(0, sprite_hub_[handle].const_buffer_->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmd_list_->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_->GetGPUDescriptorHandleForHeapStart(), handle, desc_handle_incre_size_));
	// �`��R�}���h
	cmd_list_->DrawInstanced(4, 1, 0, 0);
}

void Sprite::DrawTex(int handle, XMFLOAT2 pos, float scale)
{
	// Trans��ݒ�
	SetPos(handle, pos);
	SetSize(handle, { sprite_hub_[handle].size_.x * scale, sprite_hub_[handle].size_.y * scale });

	// �`��
	DrawTex(handle);

	// �ύX���Ă���size�����ɖ߂�
	SetSize(handle, { sprite_hub_[handle].size_.x / scale, sprite_hub_[handle].size_.y / scale });
}

void Sprite::GenerateDrawData(int handle)
{
	HRESULT result = S_FALSE;

	// �v�f���\�z
	sprite_hub_.emplace_back();

	// ���_�o�b�t�@����
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vert_num_),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite_hub_.back().vertex_buffer_));
	sprite_hub_.back().vertex_buffer_->SetName(L"SpriteVertexBuffer");
	assert(SUCCEEDED(result));

	// �e�N�X�`�����擾
	D3D12_RESOURCE_DESC res_desc = tex_buff_[handle]->GetDesc();
	// �X�v���C�g�̃T�C�Y���e�N�X�`���̃T�C�Y�ɐݒ�
	sprite_hub_.back().size_ = { (float)res_desc.Width, (float)res_desc.Height };
	sprite_hub_.back().tex_size_ = sprite_hub_.back().size_;
	TransferVertices(&sprite_hub_.back());

	// ���_�o�b�t�@�r���[�̍쐬
	sprite_hub_.back().vb_view_.BufferLocation = sprite_hub_.back().vertex_buffer_->GetGPUVirtualAddress();
	sprite_hub_.back().vb_view_.SizeInBytes = sizeof(VertexPosUv) * 4;
	sprite_hub_.back().vb_view_.StrideInBytes = sizeof(VertexPosUv);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite_hub_.back().const_buffer_));
	sprite_hub_.back().const_buffer_->SetName(L"SpriteConstantBuffer");
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *constMap = nullptr;
	result = sprite_hub_.back().const_buffer_->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = sprite_hub_.back().color_;
		constMap->mat = mat_projection_;
		sprite_hub_.back().const_buffer_->Unmap(0, nullptr);
	}

	// ���ʗp�ɊǗ�
	sprite_hub_.back().handle = handle;
}

void Sprite::TransferVertices(DrawData *itr)
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - itr->anchorpoint_.x) * itr->size_.x;
	float right = (1.0f - itr->anchorpoint_.x) * itr->size_.x;
	float top = (0.0f - itr->anchorpoint_.y) * itr->size_.y;
	float bottom = (1.0f - itr->anchorpoint_.y) * itr->size_.y;
	if (itr->is_flip_x_)
	{	// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (itr->is_flip_y_)
	{	// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	VertexPosUv vertices[vert_num_];

	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	// �e�N�X�`�����擾
	if (tex_buff_[itr->handle])
	{
		D3D12_RESOURCE_DESC res_desc = tex_buff_[itr->handle]->GetDesc();

		float tex_left = itr->tex_base_.x / res_desc.Width;
		float tex_right = (itr->tex_base_.x + itr->tex_size_.x) / res_desc.Width;
		float tex_top = itr->tex_base_.y / res_desc.Height;
		float tex_bottom = (itr->tex_base_.y + itr->tex_size_.y) / res_desc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // ����
		vertices[LT].uv = { tex_left,	tex_top }; // ����
		vertices[RB].uv = { tex_right,	tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,	tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = itr->vertex_buffer_->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		itr->vertex_buffer_->Unmap(0, nullptr);
	}
}
