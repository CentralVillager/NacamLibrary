#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "../Camera/Camera.h"

class Line {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	struct MatrixConstBufferData {

		XMMATRIX mat;
	};

	struct Vertex {

		XMFLOAT3 pos;
	};

private:

	static ComPtr<ID3D12Device> device_;
	static ComPtr<ID3D12GraphicsCommandList> command_list_;
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;
	static UINT desc_heap_size_;

private:

	ComPtr<ID3D12Resource> matrix_const_buffer_;
	Vertex matrix_const_buffer_data_;
	ComPtr<ID3D12Resource> vertex_buffer_;
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

	const XMFLOAT3 &GetPosition() { return position_; }
	const XMFLOAT3 &GetRotation() { return rotation_; }
	const XMFLOAT3 &GetScale() { return scale_; }
	void SetPosition(XMFLOAT3 position) { position_ = position; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation_ = rotation; }
	void SetScale(XMFLOAT3 scale) { this->scale_ = scale; };
	void SetScale(float scale) { this->scale_.x = this->scale_.y = this->scale_.z = scale; };

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
};
