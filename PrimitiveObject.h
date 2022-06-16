#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include "Camera.h"

class PrimitiveObject {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	// ���_���
	struct VertexData {
		XMFLOAT3 pos;	// xyz���W
	};

	// �s��p�萔�o�b�t�@
	struct MatrixConstBufferData {
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

private:	// �ÓI�����o

	// �f�o�C�X
	static ComPtr<ID3D12Device> device_;

	// �f�X�N���v�^�T�C�Y
	static UINT descriptor_handle_increment_size_;

	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> root_signature_;

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipeline_state_;

	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;

public:
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// �f�X�N���v�^�q�[�v������
	/// </summary>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// �O���t�B�N�X�p�C�v���C��������
	/// </summary>
	/// <returns></returns>
	static bool InitializeGraphicsPipeline();

private:	// �����o�ϐ�

	// �f�X�N���v�^�q�[�v
	//ComPtr<ID3D12DescriptorHeap> descriptor_heap_;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertex_buffer_;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;

	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> index_buffer_;

	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW index_buffer_view_;

	// ���_�f�[�^�z��
	std::vector<VertexData> vertices_;

	// �C���f�b�N�X�z��
	std::vector<unsigned short> indices_;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> matrix_const_buffer_;

	// �F
	XMFLOAT4 color_ = { 1, 1, 1, 1 };

	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = { 1, 1, 1 };

	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation_ = { 0, 0, 0 };

	// ���[�J�����W
	XMFLOAT3 position_ = { 0, 0, 0 };

	// ���[�J�����[���h�ϊ��s��
	XMMATRIX mat_world_;

	// �e�I�u�W�F�N�g
	PrimitiveObject *parent_ = nullptr;

	// �J�����̃|�C���^
	static Camera *cam_ptr_;

public:
	static void PreDraw();

	// -------------------------------------------------------------------------
	const XMFLOAT3 &GetPositionVector(int num) { return  vertices_[num].pos; }
	void SetPositionVector(int num, XMFLOAT3 pos) { vertices_[num].pos = pos; }

	const XMFLOAT3 &GetPosition() { return position_; }
	void SetPosition(XMFLOAT3 pos) { position_ = pos; }

	void SetColor(XMFLOAT4 color) { color_ = color; }
	// -------------------------------------------------------------------------

	void Initialize(XMFLOAT3 start, XMFLOAT3 end);
	void Finalize();
	void Update();
	void Draw();

	static void SetCamera(Camera *camera) { cam_ptr_ = camera; }

	void DrawBox(XMFLOAT2 left_upper, XMFLOAT2 right_downer, int color);

	void DrawLine();
};
