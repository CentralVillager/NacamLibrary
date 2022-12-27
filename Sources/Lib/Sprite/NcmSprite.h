#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

typedef uint32_t ncm_thandle;

class NcmSprite
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void TermSprite();

protected:
	//private:

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
		XMMATRIX mat_billboard;
	};

	/// <summary>
	/// �`��p�f�[�^
	/// </summary>
	struct DrawData
	{
		ncm_thandle handle = 0;
		const wchar_t *file_path;

		ComPtr<ID3D12Resource> vertex_buffer_;
		ComPtr<ID3D12Resource> const_buffer_;
		D3D12_VERTEX_BUFFER_VIEW vb_view_{};
		XMMATRIX mat_world_{};

		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_srv_;
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_srv_;

		XMFLOAT4 color_ = { 1, 1, 1, 1 };
		XMFLOAT3 pos_{};
		XMFLOAT3 rot_{};
		float scale_ = 1.0f;
		XMFLOAT2 size_ = { 100.0f, 100.0f };
		XMFLOAT2 anchorpoint_ = { 0, 0 };

		bool is_flip_x_ = false;
		bool is_flip_y_ = false;

		XMFLOAT2 tex_base_ = { 0, 0 };
		XMFLOAT2 tex_size_ = { 100.0f, 100.0f };
	};

protected:

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
	static ncm_thandle LoadTex(const wchar_t *filename);

	/// <summary>
	/// �摜�̕`��
	/// </summary>
	/// <param name="handle"></param>
	static void DrawTex(const ncm_thandle handle);
	static void DrawTex(const ncm_thandle handle, const XMFLOAT2 &pos);

public:

	static inline const DrawData &GetDrawData(const ncm_thandle handle) { return sprite_hub_[handle]; }
	static inline const XMFLOAT2 &GetPos(const ncm_thandle handle)
	{
		XMFLOAT2 pos = { sprite_hub_[handle].pos_.x, sprite_hub_[handle].pos_.y };
		return pos;
	}
	static inline const XMFLOAT2 &GetSize(const ncm_thandle handle) { return sprite_hub_[handle].size_; }
	static inline const XMFLOAT4 &GetColor(const ncm_thandle handle)
	{
		return sprite_hub_[handle].color_;
	}

	static inline void SetPos(const ncm_thandle handle, const XMINT2 &pos)
	{
		sprite_hub_[handle].pos_ = { (float)(pos.x), (float)(pos.y), 0 };
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetPos(const ncm_thandle handle, const XMFLOAT2 &pos)
	{
		XMFLOAT3 temp_pos = { pos.x, pos.y, 0 };
		sprite_hub_[handle].pos_ = temp_pos;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetSize(const ncm_thandle handle, const XMFLOAT2 &size)
	{
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetScale(const ncm_thandle handle, const float scale)
	{
		if (sprite_hub_[handle].scale_ == scale)
		{
			return;
		}

		XMFLOAT2 size = sprite_hub_[handle].size_;
		size.x /= sprite_hub_[handle].scale_;
		size.y /= sprite_hub_[handle].scale_;

		sprite_hub_[handle].scale_ = scale;

		size.x *= scale;
		size.y *= scale;
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetAnchorPoint(const ncm_thandle handle, const XMFLOAT2 &anchor_point)
	{
		sprite_hub_[handle].anchorpoint_ = anchor_point;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetColor(const ncm_thandle handle, XMFLOAT4 color)
	{
		sprite_hub_[handle].color_ = color;
	}

protected:
	//private:

	static void GenerateDrawData(const ncm_thandle handle, const wchar_t *filename);
	static void TransferVertices(DrawData *itr);
};
