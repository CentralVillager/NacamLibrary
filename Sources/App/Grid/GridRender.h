#pragma once
#include <array>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include <forward_list>
#include <DirectXMath.h>
#include "../../Lib/3D/Line.h"
#include "../../Lib/Camera/Camera.h"

class GridRender
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	struct VertexData
	{
		XMFLOAT3 pos;
	};

	struct MatConstBufferData
	{
		XMMATRIX mat;
	};

public:

	// �S���� * ���`��ɕK�v�Ȓ��_��
	static constexpr uint32_t VERTEX_COUNT_ = 65535 * 2;

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
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vb_view_;
	// ���_�f�[�^�z��
	static std::array<VertexData, VERTEX_COUNT_> vertices_;
	// �J�����̃|�C���^
	static Camera *cam_ptr_;

private:

	std::vector<Line> grid_;

	std::vector<VertexData> all_vert_;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> mat_const_buff_;

	// �`�撸�_��
	UINT draw_vert_size_;

	// �ʒu
	XMFLOAT3 origin_pos_;

	// z�����̃T�C�Y
	float z_size_;

	uint32_t grid_num_;
	uint32_t offset_;


public:

	GridRender();
	~GridRender();

public:

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device"></param>
	/// <param name="cmd_list"></param>
	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// ���_�o�b�t�@�̐���
	/// </summary>
	static void CreateVertexBuffer();

	/// <summary>
	/// �J�����|�C���^�̃Z�b�g
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera *camera) { cam_ptr_ = camera; }

public:

	/// <summary>
	/// �萔�o�b�t�@�̏�����
	/// </summary>
	void InitializeConstBuffer();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void UpdateVertBuffer();
	void UpdateConstBuffer();

public:

	const XMFLOAT3 &GetPos() { return origin_pos_; }
	const float GetZSize() { return z_size_; }

	const void SetZPos(float pos);

public:

	void Initialize(const UINT grid_num, const UINT offset, const XMFLOAT3 origin_pos);
	void Update(float move_speed);
	void Draw();
	void DebugDraw();

public:

	void MoveMinusZ();
};
