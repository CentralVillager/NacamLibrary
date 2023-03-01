#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include "../Camera/Camera.h"

enum class VertDesc
{
	Start,
	End,
	MaxVertNum
};

class Line
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	struct MatrixConstBufferData
	{
		XMMATRIX mat;
	};

	struct Vertex
	{
		XMFLOAT3 pos;
	};

private:

	static ComPtr<ID3D12Device> device_;
	static ComPtr<ID3D12GraphicsCommandList> command_list_;
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;
	static UINT desc_heap_size_;
	static const UINT VERT_NUM_ = (UINT)(VertDesc::MaxVertNum);
	static Camera *cam_ptr_;

private:

	ComPtr<ID3D12Resource> matrix_const_buffer_;
	ComPtr<ID3D12Resource> vertex_buffer_;
	std::array<Vertex, VERT_NUM_> vertices_data_;
	D3D12_VERTEX_BUFFER_VIEW vb_view_;

private:

	XMFLOAT3 pos_;			// ���[�J�����W
	XMFLOAT3 rot_;			// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 scale_;		// ���[�J���X�P�[��
	XMFLOAT4 color_;		// �F
	XMMATRIX mat_world_;	// ���[�J�����[���h�ϊ��s��

public:

	static void StaticInitialize();
	static void SetCamera(Camera *camera_) { cam_ptr_ = camera_; }

public:

	Line();
	~Line();

private:

	void CreateVertexBuffer();
	void CreateConstantBuffer();

public:

	inline const XMFLOAT3 &GetPosition() { return pos_; }
	inline const XMFLOAT3 &GetRotation() { return rot_; }
	inline const XMFLOAT3 &GetScale() { return scale_; }
	inline const std::array<Vertex, VERT_NUM_> &GetVertData() { return vertices_data_; }
	inline void SetPosition(XMFLOAT3 position) { pos_ = position; }
	inline void SetRotation(XMFLOAT3 rotation) { rot_ = rotation; }
	inline void SetScale(XMFLOAT3 scale) { scale_ = scale; };
	inline void SetScale(float scale) { scale_.x = scale_.y = scale_.z = scale; };
	inline void SetVertPos(XMFLOAT3 position, UINT index) { vertices_data_[index].pos = position; }

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();
};
