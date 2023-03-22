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

	// 全線数 * 線描画に必要な頂点数
	static constexpr uint32_t VERTEX_COUNT_ = 65535 * 2;

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
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vb_view_;
	// 頂点データ配列
	static std::array<VertexData, VERTEX_COUNT_> vertices_;
	// カメラのポインタ
	static Camera *cam_ptr_;

private:

	std::vector<Line> grid_;

	std::vector<VertexData> all_vert_;

	// 定数バッファ
	ComPtr<ID3D12Resource> mat_const_buff_;

	// 描画頂点数
	UINT draw_vert_size_;

	// 位置
	XMFLOAT3 origin_pos_;

	// z方向のサイズ
	float z_size_;

	uint32_t grid_num_;
	uint32_t offset_;


public:

	GridRender();
	~GridRender();

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="cmd_list"></param>
	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// 頂点バッファの生成
	/// </summary>
	static void CreateVertexBuffer();

	/// <summary>
	/// カメラポインタのセット
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera *camera) { cam_ptr_ = camera; }

public:

	/// <summary>
	/// 定数バッファの初期化
	/// </summary>
	void InitializeConstBuffer();

	/// <summary>
	/// 毎フレーム処理
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
