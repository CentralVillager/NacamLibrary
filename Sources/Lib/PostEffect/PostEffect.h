#pragma once
#include <d3d12.h>
#include <array>
#include <wrl.h>
#include <DirectXMath.h>

enum class Pipeline
{
	Basic,
	AverageBlur,
	GaussianBlur,
	RadialBlur
};

class PostEffect
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;
		XMMATRIX mat;
	};

public:

	PostEffect();
	~PostEffect();

private:

	static ComPtr<ID3D12Device> device_;
	static ComPtr<ID3D12GraphicsCommandList> command_list_;

	// 頂点数
	const static UINT vertex_num_ = 4;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertex_buffer_;

	// 定数バッファ
	ComPtr<ID3D12Resource> const_buffer_;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vb_view_;

	// テクスチャバッファ
	ComPtr<ID3D12Resource> tex_buff_[2];

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> desc_heap_SRV_;

	// デプスバッファ
	ComPtr<ID3D12Resource> depth_buff_;

	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> desc_heap_RTV_;

	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> desc_heap_DSV_;

	// 画面クリアカラー
	static const float clear_color_[4];

	static XMFLOAT4 color_;

private:

	Pipeline pipeline_;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw();

public:

	static void SetPipeline(Pipeline p);

	static float GetAlpha() { return color_.w; }
	static void SetAlpha(float color) { color_.w = color; }

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void PostDrawScene();
};
