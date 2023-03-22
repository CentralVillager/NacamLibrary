#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "../Camera/Camera.h"
#include "../2D/NcmSprite.h"

class Point
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

	struct VertexPos
	{
		XMFLOAT3 pos;
		float scale;
	};

	struct ConstBufferData
	{
		XMMATRIX mat;
		XMMATRIX mat_billboard;
	};

private:

	static constexpr int vertex_count_ = 65536;

	// デバイス
	static ComPtr<ID3D12Device> device_;
	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;
	// デスクリプタサイズ
	static UINT descriptor_handle_incre_size_;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;
	// カメラ
	static Camera *camera_;

private:

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_srv_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_srv_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vert_buff_;
	// 定数バッファ
	ComPtr<ID3D12Resource> const_buff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vb_view_;
	// ワールド行列
	XMMATRIX mat_world_;
	// テクスチャハンドル
	ncm_thandle tex_handle_;

private:

	XMFLOAT3 pos_;
	XMFLOAT3 rot_;
	float scale_;
	XMFLOAT3 color_;

public:

	Point();
	~Point();

public:

	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);
	static void SetCamera(Camera *camera) { camera_ = camera; }

	inline void SetTexHandle(ncm_thandle handle) { tex_handle_ = handle; }

	void Initialize();
	void Update();
	void Draw();

private:

	void InitializeDescriptorHeap();
	void CreateVertexBuffer();
};
