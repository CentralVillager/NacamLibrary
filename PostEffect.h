#pragma once
#include "Sprite.h"
#include <array>

class PostEffect : public Sprite {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	PostEffect();

private:

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

private:

	// 画面クリアカラー
	static const float clear_color_[4];

private:

	// グラフィクスパイプライン
	ComPtr<ID3D12PipelineState> pipeline_state_;

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> root_signature_;

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void PostDrawScene();

};
