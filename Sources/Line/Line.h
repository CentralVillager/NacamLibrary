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

private:

	ComPtr<ID3D12Resource> matrix_const_buffer_;
	ComPtr<ID3D12Resource> vertex_buffer_;
	std::array<Vertex, VERT_NUM_> vertices_data_;
	D3D12_VERTEX_BUFFER_VIEW vb_view_;

private:

	// ローカル座標
	XMFLOAT3 position_ = { 0, 0, 0 };

	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0, 0, 0 };

	// ローカルスケール
	XMFLOAT3 scale_ = { 1, 1, 1 };

	// 色
	XMFLOAT4 color_ = { 1, 1, 1, 1 };

	// ローカルワールド変換行列
	XMMATRIX mat_world_;

	// カメラのポインタ
	static Camera *cam_ptr_;

public:

	static void StaticInitialize();
	static void SetCamera(Camera *camera_) { cam_ptr_ = camera_; }

private:

	void CreateVertexBuffer();
	void CreateConstantBuffer();

public:

	inline const XMFLOAT3 &GetPosition() { return position_; }
	inline const XMFLOAT3 &GetRotation() { return rotation_; }
	inline const XMFLOAT3 &GetScale() { return scale_; }
	inline void SetPosition(XMFLOAT3 position) { position_ = position; }
	inline void SetRotation(XMFLOAT3 rotation) { this->rotation_ = rotation; }
	inline void SetScale(XMFLOAT3 scale) { this->scale_ = scale; };
	inline void SetScale(float scale) { this->scale_.x = this->scale_.y = this->scale_.z = scale; };
	inline void SetVertPos(XMFLOAT3 position, UINT index) { vertices_data_[index].pos = position; }

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();
};
