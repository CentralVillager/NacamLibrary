#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "../Camera/Camera.h"
#include "../2D/NcmSprite.h"

class PlatePoly
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	struct VertexPos
	{
		XMFLOAT3 pos;
		//XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;
		XMMATRIX mat;
		XMMATRIX mat_billboard;
	};

private:

	static constexpr int VERTEX_COUNT_ = 4;

	// �f�o�C�X
	static ComPtr<ID3D12Device> device_;
	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptor_handle_incre_size_;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;
	// �J����
	static Camera *camera_;

private:

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_srv_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_srv_;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vert_buff_;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> const_buff_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vb_view_;
	// ���[���h�s��
	XMMATRIX mat_world_;
	// �e�N�X�`���n���h��
	ncm_thandle tex_handle_;

private:

	XMFLOAT3 pos_;
	XMFLOAT3 rot_;
	float scale_;
	XMFLOAT3 color_;

public:

	PlatePoly();
	~PlatePoly();

public:

	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);
	static void SetCamera(Camera *camera) { camera_ = camera; }

	void SetTexHandle(const ncm_thandle handle) { tex_handle_ = handle; }

	inline const XMFLOAT3 &GetPos() { return pos_; }
	inline const float &GetScale() { return scale_; }
	inline void SetPos(const XMFLOAT3 &pos) { pos_ = pos; }
	inline void SetScale(const float scale) { scale_ = scale; }

public:

	void Initialize(ncm_thandle tex_handle);
	void Update();
	void Draw();

private:

	void InitializeDescriptorHeap();
	void CreateVertexBuffer(XMFLOAT2 size);
};
