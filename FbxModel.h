#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

/// <summary>
/// ノード構造体
/// </summary>
struct Node {
	// 名前
	std::string name_;
	// ローカルスケール
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	// ローカル回転角
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	// ローカル移動
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	// ローカル変形行列
	DirectX::XMMATRIX transform;
	// グローバル変形行列
	DirectX::XMMATRIX global_transform;
	// 親ノード
	Node *parent = nullptr;
};

/// <summary>
/// FBXモデルクラス
/// </summary>
class FbxModel {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using stirng = std::string;
	template <class T> using vector = std::vector<T>;

public:
	// フレンドクラス
	friend class FbxLoader;

public:	// 定数
	static const int MAX_BONE_INDICES_ = 4;

public:
	// 頂点データ構造体
	struct VertexData {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT bone_index[MAX_BONE_INDICES_];		// ボーン番号
		float bone_weight[MAX_BONE_INDICES_];	// ボーン重み
	};

	// ボーン構造体
	struct Bone {
		// 名前
		std::string name;
		// 初期姿勢の逆行列
		DirectX::XMMATRIX inverse_initial_pose;
		// クラスター(FBX側のボーン情報)
		FbxCluster *fbx_cluster;

		// コンストラクタ
		Bone(const std::string &name) { this->name = name; }
	};

private:
	// 頂点バッファ
	ComPtr<ID3D12Resource> vert_buff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> index_buff_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> tex_buff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vb_view = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ib_view = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> desc_heap_srv;

public:
	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="device"></param>
	void CreateBuffers(ID3D12Device *device);

private:
	// モデル名
	std::string name_;
	// ノード配列
	std::vector<Node> nodes_;
	// メッシュを持つノード
	Node *mesh_node_ = nullptr;
	// 頂点データ配列
	std::vector<VertexData> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;

	// アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
	// ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
	// テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	// スクラッチイメージ
	DirectX::ScratchImage scratch_img = {};

	// ボーン配列
	std::vector<Bone> bones_;

	// FBXシーン
	FbxScene *fbx_scene_ = nullptr;

public:
	~FbxModel();

	const XMMATRIX &GetModelTransform() { return mesh_node_->global_transform; }
	std::vector<Bone> &GetBones() { return bones_; }
	FbxScene *GetFbxScene() { return fbx_scene_; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmd_list"></param>
	void Draw(ID3D12GraphicsCommandList *cmd_list);
};
