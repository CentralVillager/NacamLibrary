#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <DirectXTex.h>
#include "DirectXBase.h"
#include <unordered_map>

class Model {
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// 頂点データ
	struct VertexData {
		XMFLOAT3 position;	// xyz座標
		XMFLOAT3 normal;	// 法線ベクトル
		XMFLOAT2 uv;		// uv座標
	};

	// マテリアルデータ
	struct MaterialData {
		std::string name_;				// マテリアル名
		XMFLOAT3 ambient;				// アンビエント影響度
		XMFLOAT3 diffuse;				// ディフューズ影響度
		XMFLOAT3 specular;				// スペキュラー影響度
		float alpha;					// アルファ
		std::string texture_file_name;	// テクスチャファイル名

		// コンストラクタ
		MaterialData() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	}material_data_;

	// マテリアル定数バッファ用データ構造体
	struct MaterialConstBufferData {
		XMFLOAT3 ambient;	// アンビエント係数
		float pad1;			// パディング
		XMFLOAT3 diffuse;	// ディフューズ係数
		float pad2;			// パディング
		XMFLOAT3 specular;	// スペキュラー係数
		float alpha;		// アルファ
	};

	// テクスチャ
	struct TextureData {
		// テクスチャバッファ
		ComPtr<ID3D12Resource> texture_buffer;
		// マテリアル用の定数バッファ
		MaterialConstBufferData const_buffer;
		// デスクリプタテーブル
		ComPtr<ID3D12RootSignature> rootSignature;
		// シェーダリソースビューのハンドル(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_SRV;
		// シェーダリソースビューのハンドル(GPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_SRV;
	}tex_data_;

	// 頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smooth_data_;

private:
	static DirectXBase *dx_base_;
	static ComPtr<ID3D12Device> device_;
	// デスクリプタサイズ
	static UINT descriptor_handle_increment_size_;

	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
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

public:
	Model();

	MaterialData &GetMaterialData() { return material_data_; }
	ComPtr<ID3D12Resource> &GetVB() { return vertex_buffer_; }
	ComPtr<ID3D12Resource> &GetIB() { return index_buffer_; }
	D3D12_VERTEX_BUFFER_VIEW GetVBView() { return vertex_buffer_view_; }
	D3D12_INDEX_BUFFER_VIEW GetIBView() { return index_buffer_view_; }
	const vector<VertexData> &GetVertices() { return vertices_; }
	vector<unsigned short> GetIndices() { return indices_; }
	size_t GetNumIndices() { size_t num = indices_.size(); return num; }
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return descriptor_heap_; }
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuDescHandleSRV() { return tex_data_.gpu_desc_handle_SRV; }

	static void StaticInitialize();

	/// <summary>
	/// Objファイルの読み込み
	/// </summary>
	/// <param name="directory_path"></param>
	/// <param name="file_name"></param>
	/// <returns></returns>
	void LoadObjModel(const std::string &directory_path, const std::string &file_name, const std::string &material_name);

	/// <summary>
	/// 初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadModel(const std::string &directoryPath, const std::string &filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	bool LoadTexture(const std::string &directoryPath, const std::string &filename);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string &directoryPath, const std::string &filename);
};
