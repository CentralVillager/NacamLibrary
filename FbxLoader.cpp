#include "FbxLoader.h"
#include <cassert>

using namespace DirectX;

const std::string FbxLoader::base_directory_ = "Resources/";
const std::string FbxLoader::default_texture_file_name_ = "white1x1.png";

void FbxLoader::Initialize(ID3D12Device *device) {
	// 再初期化チェック
	assert(fbx_manager_ == nullptr);
	// 引数からメンバ変数に代入
	device_ = device;
	// FBXマネージャの生成
	fbx_manager_ = FbxManager::Create();
	// FBXマネージャの入出力設定
	FbxIOSettings *ios = FbxIOSettings::Create(fbx_manager_, IOSROOT);
	fbx_importer_ = FbxImporter::Create(fbx_manager_, "");
}

void FbxLoader::Finalize() {
	// 各種FBXインスタンスの破棄
	fbx_importer_->Destroy();
	fbx_manager_->Destroy();
}

FbxModel *FbxLoader::LoadModelFromFile(const string &model_name) {
	// モデルと同じ名前のフォルダから読み込む
	const string directory_path = base_directory_ + model_name + "/";
	// 拡張子.fbxを追加
	const string file_name = model_name + ".fbx";
	// 連結してフルパスを得る
	const string fullpath = directory_path + file_name;

	// ファイル名を指定して.fbxファイルを読み込む
	if (!fbx_importer_->Initialize(fullpath.c_str(), -1, fbx_manager_->GetIOSettings())) {
		assert(0);
	}

	// シーン生成
	FbxScene *fbx_scene = FbxScene::Create(fbx_manager_, "fbxScene");

	// ファイルからロードしたFBXの情報をシーンにインポート
	fbx_importer_->Import(fbx_scene);

	// モデル生成
	FbxModel *model = new FbxModel();
	model->name_ = model_name;
	// FBXノードの数を取得
	int node_count = fbx_scene->GetNodeCount();
	// あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
	model->nodes_.reserve(node_count);
	// ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(model, fbx_scene->GetRootNode());
	// FBXシーン解放
	//fbx_scene->Destroy();
	model->fbx_scene_ = fbx_scene;

	// バッファ生成
	model->CreateBuffers(device_.Get());

	return model;
}

void FbxLoader::ParseNodeRecursive(FbxModel *model, FbxNode *fbx_node, Node *parent) {
	// ノード名を取得
	string name = fbx_node->GetName();
	// モデルにノードを追加
	model->nodes_.emplace_back();
	Node &node = model->nodes_.back();
	// ノード名を取得
	node.name_ = fbx_node->GetName();

	// FBXノードのローカル移動情報
	FbxDouble3 rotation = fbx_node->LclRotation.Get();
	FbxDouble3 scaling = fbx_node->LclScaling.Get();
	FbxDouble3 translation = fbx_node->LclTranslation.Get();

	// 形式変換して代入
	node.rotation = { static_cast<float>(rotation[0]), static_cast<float>(rotation[1]), static_cast<float>(rotation[2]), 0.0f };
	node.scaling = { static_cast<float>(scaling[0]), static_cast<float>(scaling[1]), static_cast<float>(scaling[2]), 0.0f };
	node.translation = { static_cast<float>(translation[0]), static_cast<float>(translation[1]), static_cast<float>(translation[2]), 1.0f };

	// スケール、回転、平行移動行列の計算
	XMMATRIX mat_scaling, mat_rotation, mat_translation;
	mat_scaling = XMMatrixScalingFromVector(node.scaling);
	mat_rotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	mat_translation = XMMatrixTranslationFromVector(node.translation);

	// ローカル変形行列の計算
	node.transform = XMMatrixIdentity();
	node.transform *= mat_scaling;
	node.transform *= mat_rotation;
	node.transform *= mat_translation;

	// グローバル変形行列の計算
	node.global_transform = node.transform;
	if (parent) {
		node.parent = parent;
		// 親の変形を乗算
		node.global_transform *= parent->global_transform;
	}

	// FBXノードのメッシュ情報を解析
	FbxNodeAttribute *fbx_node_attibute = fbx_node->GetNodeAttribute();

	if (fbx_node_attibute) {
		if (fbx_node_attibute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			model->mesh_node_ = &node;
			ParseMesh(model, fbx_node);

			// ノードからモデルに変換？
			for (int i = 0; i < model->vertices_.size(); i++) {
				XMVECTOR vector = XMVector3Transform(XMLoadFloat3(&model->vertices_[i].pos), node.transform);
				XMStoreFloat3(&model->vertices_[i].pos, vector);
			}
		}
	}

	// 子ノードに対して再帰呼び出し
	for (int i = 0; i < fbx_node->GetChildCount(); i++) {
		ParseNodeRecursive(model, fbx_node->GetChild(i), &node);
	}
}

void FbxLoader::ParseMesh(FbxModel *model, FbxNode *fbx_node) {
	// ノードのメッシュを取得
	FbxMesh *fbx_mesh = fbx_node->GetMesh();

	// 頂点座標読み取り
	ParseMeshVertices(model, fbx_mesh);
	// 面を構成するデータの読み取り
	ParseMeshFaces(model, fbx_mesh);
	// マテリアルの読み込み
	ParseMaterial(model, fbx_node);
	// スキニング情報の読み取り
	ParseSkin(model, fbx_mesh);
}

void FbxLoader::ParseMeshVertices(FbxModel *model, FbxMesh *fbx_mesh) {
	auto &vertices = model->vertices_;

	// 頂点座標データの数
	const int control_points_count = fbx_mesh->GetControlPointsCount();
	// 必要数だけ頂点データ配列を確保
	FbxModel::VertexData vert{};
	model->vertices_.resize(control_points_count, vert);

	// FBXメッシュの頂点座標配列を取得
	FbxVector4 *p_coord = fbx_mesh->GetControlPoints();
	// FBXメッシュの全頂点座標をモデル内の配列にコピーする
	for (int i = 0; i < control_points_count; i++) {
		FbxModel::VertexData &vertex = vertices[i];
		// 座標のコピー
		vertex.pos.x = static_cast<float>(p_coord[i][0]);
		vertex.pos.y = static_cast<float>(p_coord[i][1]);
		vertex.pos.z = static_cast<float>(p_coord[i][2]);
	}
}

void FbxLoader::ParseMeshFaces(FbxModel *model, FbxMesh *fbx_mesh) {
	auto &vertices = model->vertices_;
	auto &indices = model->indices_;

	// 1ファイルに複数メッシュのモデルは非対応
	assert(indices.size() == 0);
	// 面の数
	const int polygon_count = fbx_mesh->GetPolygonCount();
	// UVデータの数
	const int texture_uv_count = fbx_mesh->GetTextureUVCount();
	// UV各リスト
	FbxStringList uv_names;
	fbx_mesh->GetUVSetNames(uv_names);
	// 面ごとの情報読み取り
	for (int i = 0; i < polygon_count; i++) {
		// 面を構成する頂点の数を所得（3なら三角ポリゴン）
		const int polygon_size = fbx_mesh->GetPolygonSize(i);
		assert(polygon_size <= 4);

		// 1頂点ずつ処理
		for (int j = 0; j < polygon_size; j++) {
			// FBX頂点配列のインデックス
			int index = fbx_mesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			// 頂点法線読み込み
			FbxModel::VertexData &vertex = vertices[index];
			FbxVector4 normal;
			if (fbx_mesh->GetPolygonVertexNormal(i, j, normal)) {
				vertex.normal.x = static_cast<float>(normal[0]);
				vertex.normal.y = static_cast<float>(normal[1]);
				vertex.normal.z = static_cast<float>(normal[2]);
			}
			// テクスチャUV読み込み
			if (texture_uv_count > 0) {
				FbxVector2 uvs;
				bool l_unmapped_uv;
				// 0番決め打ちで読み込み
				if (fbx_mesh->GetPolygonVertexUV(i, j, uv_names[0], uvs, l_unmapped_uv)) {
					vertex.uv.x = static_cast<float>(uvs[0]);
					vertex.uv.y = static_cast<float>(uvs[1]);
				}
			}
			// インデックス配列に頂点インデックスを追加
			// 3頂点までなら
			if (j < 3) {
				// 1点追加し、他の2点と三角形を構築する
				indices.push_back(index);
			} else {
				//4頂点目
				// 3点追加し、
				// 四角形の0, 1, 2, 3の内、2, 3, 0で三角形を構築する
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}
}

void FbxLoader::ParseMaterial(FbxModel *model, FbxNode *fbx_node) {
	const int material_count = fbx_node->GetMaterialCount();
	if (material_count > 0) {
		// 先頭のマテリアルを取得
		FbxSurfaceMaterial *material = fbx_node->GetMaterial(0);
		// テクスチャを読み込んだかどうかを表すフラグ
		bool texture_loaded = false;

		if (material) {
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
				FbxSurfaceLambert *lambert = static_cast<FbxSurfaceLambert *>(material);

				// 環境光係数
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				model->ambient.x = static_cast<float>(ambient.Get()[0]);
				model->ambient.y = static_cast<float>(ambient.Get()[1]);
				model->ambient.z = static_cast<float>(ambient.Get()[2]);

				// 拡散反射光係数
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				model->diffuse.x = static_cast<float>(diffuse.Get()[0]);
				model->diffuse.y = static_cast<float>(diffuse.Get()[1]);
				model->diffuse.z = static_cast<float>(diffuse.Get()[2]);
			}

			// ディフューズテクスチャを取り出す
			const FbxProperty diffuse_property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (diffuse_property.IsValid()) {
				const FbxFileTexture *texture = diffuse_property.GetSrcObject<FbxFileTexture>();
				if (texture) {
					const char *file_path = texture->GetFileName();
					// ファイルパスからファイル名抽出
					string path_str(file_path);
					string name = ExtractFileName(path_str);
					// テクスチャ読み込み
					LoadTexture(model, base_directory_ + model->name_ + "/" + name + ".png");
					texture_loaded = true;
				}
			}
		}
		// テクスチャがない場合は白テクスチャを貼る
		if (!texture_loaded) {
			LoadTexture(model, base_directory_ + default_texture_file_name_);
		}
	}
}

void FbxLoader::LoadTexture(FbxModel *model, const std::string &fullpath) {
	HRESULT result = S_FALSE;
	// WICテクスチャのロード
	TexMetadata &metadata = model->metadata;
	ScratchImage &scratch_img = model->scratch_img;
	// ユニコード文字列に変換
	wchar_t w_file_path[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, w_file_path, _countof(w_file_path));

	result = LoadFromWICFile(w_file_path, WIC_FLAGS_NONE, &metadata, scratch_img);
	if (FAILED(result)) {
		assert(0);
	}
}

std::string FbxLoader::ExtractFileName(const std::string &path) {
	size_t pos1;
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src) {
	// 行
	for (int i = 0; i < 4; i++) {
		// 列
		for (int j = 0; j < 4; j++) {
			// 1要素コピー
			dst->r[i].m128_f32[j] = static_cast<float>(src.Get(i, j));
		}
	}
}

void FbxLoader::ParseSkin(FbxModel *model, FbxMesh *fbx_mesh) {
	// スキニング情報
	FbxSkin *fbx_skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(0, FbxDeformer::eSkin));
	// スキニング情報が無ければ終了
	if (fbx_skin == nullptr) {
		// 各頂点について処理
		for (int i = 0; i < model->vertices_.size(); i++) {
			// 最初のボーン（単位行列）の影響100%にする
			model->vertices_[i].bone_index[0] = 0;
			model->vertices_[i].bone_weight[0] = 1.0f;
		}

		return;
	}

	// ボーン配列の参照
	std::vector<FbxModel::Bone> &bones = model->bones_;

	// ボーンの数
	int cluster_count = fbx_skin->GetClusterCount();
	bones.reserve(cluster_count);

	// 全てのボーンについて
	for (int i = 0; i < cluster_count; i++) {
		// FBXボーン情報
		FbxCluster *fbx_cluster = fbx_skin->GetCluster(i);

		// ボーン自体のノードの名前を取得
		const char *bone_name = fbx_cluster->GetLink()->GetName();

		// 新しくボーンを追加し、追加したボーンの参照を得る
		bones.emplace_back(FbxModel::Bone(bone_name));
		FbxModel::Bone &bone = bones.back();
		// 自作ボーンとFBXのボーンを紐づける
		bone.fbx_cluster = fbx_cluster;

		// FBXから初期姿勢行列を取得する
		FbxAMatrix fbx_mat;
		fbx_cluster->GetTransformLinkMatrix(fbx_mat);

		// XMMATRIX型に変換する
		XMMATRIX initial_pose;
		ConvertMatrixFromFbx(&initial_pose, fbx_mat);

		// 初期姿勢行列の逆行列を得る
		bone.inverse_initial_pose = XMMatrixInverse(nullptr, initial_pose);
	}

	// ボーン番号とスキンウェイトのペア
	struct WeightSet {
		UINT index;
		float weight;
	};

	// 二次元配列（ジャグ配列）
	// list:	頂点が影響を受けるボーンの全リスト
	// vector:	それを全頂点分
	std::vector<std::list<WeightSet>> weight_lists(model->vertices_.size());

	// 全てのボーンについて
	for (int i = 0; i < cluster_count; i++) {
		// FBXボーン情報
		FbxCluster *fbx_cluster = fbx_skin->GetCluster(i);
		// このボーンに影響を受ける頂点の数
		int control_point_indices_count = fbx_cluster->GetControlPointIndicesCount();
		// このボーンに影響を受ける頂点の配列
		int *control_point_indices = fbx_cluster->GetControlPointIndices();
		double *control_point_weights = fbx_cluster->GetControlPointWeights();

		// 影響を受ける全頂点について
		for (int j = 0; j < control_point_indices_count; j++) {
			// 頂点番号
			int vert_index = control_point_indices[j];
			// スキンウェイト
			float weight = static_cast<float>(control_point_weights[j]);
			// その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
			weight_lists[vert_index].emplace_back(WeightSet{ static_cast<UINT>(i), weight });
		}
	}

	// 頂点配列書き換え用の参照
	auto &vertices = model->vertices_;
	// 各頂点について処理
	for (int i = 0; i < vertices.size(); i++) {
		// 頂点のウェイトから最も大きい4つを選択
		auto &weight_list = weight_lists[i];
		// 大小比較用のラムダ式を指定して降順にソート
		weight_list.sort([](auto const &lhs, auto const &rhs) {
			// 左の要素の方が大きければtrue, そうでなければfalseを返す
			return lhs.weight > rhs.weight;
						 });

		int weight_array_index = 0;
		// 降順ソート済みのウェイトリストから
		for (auto &weight_set : weight_list) {
			// 頂点データに書き込み
			vertices[i].bone_index[weight_array_index] = weight_set.index;
			vertices[i].bone_weight[weight_array_index] = weight_set.weight;
			// 4つに達したら終了
			if (++weight_array_index >= FbxModel::MAX_BONE_INDICES_) {
				float weight = 0.0f;
				// 2番目以降のウェイトを合計
				for (int j = 1; j < FbxModel::MAX_BONE_INDICES_; j++) {
					weight += vertices[i].bone_weight[j];
				}
				// 合計で1.0f(100%)になるように調整
				vertices[i].bone_weight[0] = 1.0f - weight;
				break;
			}
		}
	}
}
