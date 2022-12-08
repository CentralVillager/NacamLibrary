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

	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos;	// xyz座標
		float scale;	// スケール
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		float alpha;	// 透明度
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX mat_billboard;
	};

private:

	static constexpr int VERTEX_COUNT_ = 65536;	// 頂点数

private:

	// デバイス
	static ID3D12Device *device_;
	// デスクリプタサイズ
	static UINT desc_incre_size_;
	// コマンドリスト
	static ID3D12GraphicsCommandList *cmd_list_;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vert_buff_;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> tex_buff_;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_srv_;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_srv_;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vb_view_;
	// 頂点データ配列
	static VertexPos vertices_[VERTEX_COUNT_];
	// カメラのポインタ
	static Camera *cam_ptr_;

private:

	// 定数バッファ
	ComPtr<ID3D12Resource> const_buff_;

	// 描画頂点数
	UINT draw_vert_size_;

public:

	NcmPlatePoly();
	~NcmPlatePoly();

private:

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	static bool LoadTexture();

	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// カメラのポインタのセット
	/// </summary>
	/// <param name="cam"></param>
	static void SetCamera(Camera *cam) { cam_ptr_ = cam; }

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void UpdateVertBuffer(std::forward_list<NcmParticleCommonArgs> &args);
	void UpdateConstBuffer(std::forward_list<NcmParticleCommonArgs> &args);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
};
