#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include "fbxsdk.h"
#include "FbxModel.h"
#include "../Singleton/Singleton.h"

class FbxLoader : public Singleton<FbxLoader> {
	friend Singleton<FbxLoader>;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;

	ComPtr<ID3D12Device> device_ = nullptr;
	FbxManager *fbx_manager_ = nullptr;
	FbxImporter *fbx_importer_ = nullptr;

	// テクスチャがない場合の標準テクスチャファイル名
	static const string default_texture_file_name_;

public:
	// モデル格納ルートパス
	static const string base_directory_;

	void Initialize(ID3D12Device *device);
	void Finalize();

	/// <summary>
	/// FBXファイルの読み込み
	/// </summary>
	/// <param name="model_name">モデル名</param>
	FbxModel *LoadModelFromFile(const string &model_name);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="fbx_model">読み込み先モデルオブジェクト</param>
	/// <param name="fbx_node">解析対象のノード</param>
	void ParseNodeRecursive(FbxModel *model, FbxNode *fbx_node, Node *parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbx_node">解析対象のノード</param>
	void ParseMesh(FbxModel *model, FbxNode *fbx_node);

	// 頂点座標読み取り
	void ParseMeshVertices(FbxModel *model, FbxMesh *fbx_mesh);
	// 面情報読み取り
	void ParseMeshFaces(FbxModel *model, FbxMesh *fbx_mesh);
	// マテリアル読み取り
	void ParseMaterial(FbxModel *model, FbxNode *fbx_node);
	// テクスチャ読み込み
	void LoadTexture(FbxModel *model, const std::string &fullpath);

	// ディレクトリを含んだファイルパス名を検出する
	std::string ExtractFileName(const std::string &path);

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src);

	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbx_mesh"></param>
	void ParseSkin(FbxModel *model, FbxMesh *fbx_mesh);
};