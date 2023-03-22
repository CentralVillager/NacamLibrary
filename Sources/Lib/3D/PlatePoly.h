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
	// シェーダリソースビューのハンドル(GPU)
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
