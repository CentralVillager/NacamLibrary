#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

class Sprite
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void TermSprite();

private:

	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	/// <summary>
	/// 定数バッファ用データ構造体
	/// </summary>
	struct ConstBufferData
	{
		XMFLOAT4 color;
		XMMATRIX mat;
	};

	/// <summary>
	/// 描画用データ
	/// </summary>
	struct DrawData
	{
		int handle = 0;

		ComPtr<ID3D12Resource> vertex_buffer_;
		ComPtr<ID3D12Resource> const_buffer_;
		D3D12_VERTEX_BUFFER_VIEW vb_view_{};
		XMMATRIX mat_world_{};

		XMFLOAT4 color_ = { 1, 1, 1, 1 };
		XMFLOAT2 position_{};
		float rotation_ = 0.0f;
		XMFLOAT2 size_ = { 100.0f, 100.0f };
		XMFLOAT2 anchorpoint_ = { 0, 0 };

		bool is_flip_x_ = false;
		bool is_flip_y_ = false;

		XMFLOAT2 tex_base_ = { 0, 0 };
		XMFLOAT2 tex_size_ = { 100.0f, 100.0f };
	};

private:

	// テクスチャの最大枚数
	static const int srv_count_ = 512;

	// 頂点数
	static const int vert_num_ = 4;

	// デバイス
	static ComPtr<ID3D12Device> device_;

	// デスクリプタサイズ
	static UINT desc_handle_incre_size_;

	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	// 射影行列
	static XMMATRIX mat_projection_;

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;

	// テクスチャバッファ
	static ComPtr<ID3D12Resource> tex_buff_[srv_count_];

	// 総ハンドルカウンター
	static UINT handle_counter_;

	// 全スプライト情報の配列
	static std::vector<DrawData> sprite_hub_;

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="cmd_list"></param>
	static void StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// 画像の読み込み
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	static int LoadTex(const wchar_t *filename);

	/// <summary>
	/// 画像の描画
	/// </summary>
	/// <param name="handle"></param>
	static void DrawTex(const int handle);
	static void DrawTex(const int handle, const XMFLOAT2 &pos, const float scale = 1.0f);

public:

	static inline const XMFLOAT2 &GetPos(const int handle) { return sprite_hub_[handle].position_; }
	static inline const XMFLOAT2 &GetSize(const int handle) { return sprite_hub_[handle].size_; }

	static inline void SetPos(const int handle, const XMINT2 &pos)
	{
		sprite_hub_[handle].position_ = { (float)(pos.x), (float)(pos.y) };
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetPos(const int handle, const XMFLOAT2 &pos)
	{
		sprite_hub_[handle].position_ = pos;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetSize(const int handle, const XMFLOAT2 &size)
	{
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetScale(const int handle, const float scale)
	{
		XMFLOAT2 size = sprite_hub_[handle].size_;
		size.x *= scale;
		size.y *= scale;
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void ResetScale(const int handle, const float scale)
	{
		XMFLOAT2 size = sprite_hub_[handle].size_;
		size.x /= scale;
		size.y /= scale;
		sprite_hub_[handle].size_ = size;
		TransferVertices(&sprite_hub_[handle]);
	}
	static inline void SetAnchorPoint(const int handle, const XMFLOAT2 &anchor_point)
	{
		sprite_hub_[handle].anchorpoint_ = anchor_point;
		TransferVertices(&sprite_hub_[handle]);
	}

private:

	static void GenerateDrawData(const int handle);
	static void TransferVertices(DrawData *itr);
};
