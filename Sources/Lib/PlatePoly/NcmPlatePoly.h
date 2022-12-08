#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "../../Lib/Camera/Camera.h"
#include "../../App/Particle/NcmParticleCommonArgs.h"

class NcmPlatePoly
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos;	// xyz���W
		float scale;	// �X�P�[��
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		float alpha;	// �����x
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX mat_billboard;
	};

private:

	static constexpr int VERTEX_COUNT_ = 65536;	// ���_��

private:

	// �f�o�C�X
	static ID3D12Device *device_;
	// �f�X�N���v�^�T�C�Y
	static UINT desc_incre_size_;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList *cmd_list_;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vert_buff_;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> tex_buff_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_srv_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_srv_;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vb_view_;
	// ���_�f�[�^�z��
	static VertexPos vertices_[VERTEX_COUNT_];
	// �J�����̃|�C���^
	static Camera *cam_ptr_;

private:

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> const_buff_;

	// �`�撸�_��
	UINT draw_vert_size_;

public:

	NcmPlatePoly();
	~NcmPlatePoly();

private:

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	static bool LoadTexture();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

public:

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <returns>����</returns>
	static bool StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// �J�����̃|�C���^�̃Z�b�g
	/// </summary>
	/// <param name="cam"></param>
	static void SetCamera(Camera *cam) { cam_ptr_ = cam; }

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void UpdateVertBuffer(std::forward_list<NcmParticleCommonArgs> &args);
	void UpdateConstBuffer(std::forward_list<NcmParticleCommonArgs> &args);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
};