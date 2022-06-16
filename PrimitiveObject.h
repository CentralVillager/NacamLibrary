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

	// 頂点情報
	struct VertexData {
		XMFLOAT3 pos;	// xyz座標
	};

	// 行列用定数バッファ
	struct MatrixConstBufferData {
		XMMATRIX mat;	// ３Ｄ変換行列
	};

private:	// 静的メンバ

	// デバイス
	static ComPtr<ID3D12Device> device_;

	// デスクリプタサイズ
	static UINT descriptor_handle_increment_size_;

	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> root_signature_;

	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipeline_state_;

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// デスクリプタヒープ初期化
	/// </summary>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// グラフィクスパイプライン初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeGraphicsPipeline();

private:	// メンバ変数

	// デスクリプタヒープ
	//ComPtr<ID3D12DescriptorHeap> descriptor_heap_;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertex_buffer_;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;

	// インデックスバッファ
	ComPtr<ID3D12Resource> index_buffer_;

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW index_buffer_view_;

	// 頂点データ配列
	std::vector<VertexData> vertices_;

	// インデックス配列
	std::vector<unsigned short> indices_;

	// 定数バッファ
	ComPtr<ID3D12Resource> matrix_const_buffer_;

	// 色
	XMFLOAT4 color_ = { 1, 1, 1, 1 };

	// ローカルスケール
	XMFLOAT3 scale_ = { 1, 1, 1 };

	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0, 0, 0 };

	// ローカル座標
	XMFLOAT3 position_ = { 0, 0, 0 };

	// ローカルワールド変換行列
	XMMATRIX mat_world_;

	// 親オブジェクト
	PrimitiveObject *parent_ = nullptr;

	// カメラのポインタ
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
