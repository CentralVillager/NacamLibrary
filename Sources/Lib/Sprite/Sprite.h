#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

class Sprite
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void TermSprite();

private:

	/// <summary>
	/// ���_�f�[�^�\����
	/// </summary>
	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	/// <summary>
	/// �萔�o�b�t�@�p�f�[�^�\����
	/// </summary>
	struct ConstBufferData
	{
		XMFLOAT4 color;
		XMMATRIX mat;
	};

	/// <summary>
	/// �`��p�f�[�^
	/// </summary>
	struct DrawData
	{
		int handle = 0;

		ComPtr<ID3D12Resource> vertex_buffer_;
		ComPtr<ID3D12Resource> const_buffer_;
		D3D12_VERTEX_BUFFER_VIEW vb_view_{};
		XMMATRIX mat_world_{};

		XMFLOAT4 color_ = { 1, 1, 1, 1 };
		XMFLOAT2 position_{};
		float rotation_ = 0.0f;
		XMFLOAT2 size_ = { 100.0f, 100.0f };
		XMFLOAT2 anchorpoint_ = { 0, 0 };

		bool is_flip_x_ = false;
		bool is_flip_y_ = false;

		XMFLOAT2 tex_base_ = { 0, 0 };
		XMFLOAT2 tex_size_ = { 100.0f, 100.0f };
	};

private:

	// �e�N�X�`���̍ő喇��
	static const int srv_count_ = 512;

	// ���_��
	static const int vert_num_ = 4;

	// �f�o�C�X
	static ComPtr<ID3D12Device> device_;

	// �f�X�N���v�^�T�C�Y
	static UINT desc_handle_incre_size_;

	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	// �ˉe�s��
	static XMMATRIX mat_projection_;

	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;

	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> tex_buff_[srv_count_];

	// ���n���h���J�E���^�[
	static UINT handle_counter_;

	// �S�X�v���C�g���̔z��
	static std::vector<DrawData> sprite_hub_;

public:

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device"></param>
	/// <param name="cmd_list"></param>
	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// �摜�̓ǂݍ���
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	static int LoadTex(const wchar_t *filename);

	/// <summary>
	/// �摜�̕`��
	/// </summary>
	/// <param name="handle"></param>
	static void DrawTex(const int handle);
	static void DrawTex(const int handle, const XMFLOAT2 &pos, const float scale = 1.0f);

public:

	static inline const XMFLOAT2 &GetPos(const int handle) { return sprite_hub_[handle].position_; }
	static inline const XMFLOAT2 &GetSize(const int handle) { return sprite_hub_[handle].size_; }

	static inline void SetPos(const int handle, const XMINT2 &pos)
	{
		sprite_hub_[handle].position_ = { (float)(pos.x), (float)(pos.y) };
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetPos(const int handle, const XMFLOAT2 &pos)
	{
		sprite_hub_[handle].position_ = pos;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetSize(const int handle, const XMFLOAT2 &size)
	{
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetScale(const int handle, const float scale)
	{
		XMFLOAT2 size = sprite_hub_[handle].size_;
		size.x *= scale;
		size.y *= scale;
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void ResetScale(const int handle, const float scale)
	{
		XMFLOAT2 size = sprite_hub_[handle].size_;
		size.x /= scale;
		size.y /= scale;
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetAnchorPoint(const int handle, const XMFLOAT2 &anchor_point)
	{
		sprite_hub_[handle].anchorpoint_ = anchor_point;
		TransferVertices(&sprite_hub_[handle]);
	}

private:

	static void GenerateDrawData(const int handle);
	static void TransferVertices(DrawData *itr);
};
