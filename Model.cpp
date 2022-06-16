#include "Model.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

DirectXBase *Model::dx_base_;
ComPtr<ID3D12Device> Model::device_;
UINT Model::descriptor_handle_increment_size_;

Model::Model() {
}

void Model::StaticInitialize() {
	dx_base_ = DirectXBase::GetInstance();
	device_ = dx_base_->GetDevice().Get();
}

void Model::LoadObjModel(const std::string &directory_path, const std::string &file_name, const std::string &material_name) {
	InitializeDescriptorHeap();
	LoadModel(directory_path, file_name);
	LoadMaterial(directory_path, material_name);
}

void Model::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptor_heap_));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptor_handle_increment_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::LoadModel(const std::string &directoryPath, const std::string &filename) {
	HRESULT result = S_FALSE;

	// ファイルストリーム
	std::ifstream file;
	// objファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// 頂点座標
	vector<XMFLOAT3> positions;
	// 法線ベクトル
	vector<XMFLOAT3> normals;
	// テクスチャUV
	vector<XMFLOAT2> texcoords;

	// 1行ずつ読み込む
	string line;
	while (getline(file, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭文字列がmtllibならマテリアル
		if (key == "mtl") {
			// マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		// 先頭文字列が	vなら頂点座標
		if (key == "v") {
			// X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// 座標データに追加
			positions.emplace_back(position);

			// 頂点座標に追加
			/*VertexPosNormalUv vertex{};
			vertex.pos = position;
			vertices.emplace_back(vertex);*/
		}

		// 先頭文字列が	vtならテクスチャ
		if (key == "vt") {
			// U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		// 先頭文字列がvnならベクトル
		if (key == "vn") {
			// x,y,z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字がfならポリゴン（三角形）
		if (key == "f") {
			// 半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short index_position, index_normal, index_texcoord;

				index_stream >> index_position;
				index_stream.seekg(1, ios_base::cur);	// スラッシュを飛ばす
				index_stream >> index_texcoord;
				index_stream.seekg(1, ios_base::cur);	// スラッシュを飛ばす
				index_stream >> index_normal;

				// 頂点データの追加
				VertexData vertex{};
				vertex.position = positions[index_position - 1];
				vertex.normal = normals[index_normal - 1];
				vertex.uv = texcoords[index_texcoord - 1];
				vertices_.emplace_back(vertex);

				// インデックスデータの追加
				indices_.emplace_back(static_cast<unsigned short>(indices_.size()));

				// 頂点インデックスに追加
				//indices.emplace_back(index_position - 1);
			}
		}
	}
	file.close();

	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices_.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	DirectXBase *dx_base_ = DirectXBase::GetInstance();

	// 頂点バッファ生成
	result = dx_base_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = dx_base_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buffer_));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexData *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vert_map);
		vertex_buffer_->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short *indexMap = nullptr;
	result = index_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&indexMap));
	if (SUCCEEDED(result)) {
		// 全インデックスに対して
		std::copy(indices_.begin(), indices_.end(), indexMap);
		index_buffer_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeVB;
	vertex_buffer_view_.StrideInBytes = sizeof(vertices_[0]);

	// インデックスバッファビューの作成
	index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
	index_buffer_view_.Format = DXGI_FORMAT_R16_UINT;
	index_buffer_view_.SizeInBytes = sizeIB;
}

bool Model::LoadTexture(const std::string &directoryPath, const std::string &filename) {
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// ファイルパスを結合
	string filepath = directoryPath + filename;

	// ユニコード文字列に変換する
	wchar_t w_filepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(),
										  -1, w_filepath, _countof(w_filepath));

	result = LoadFromWICFile(w_filepath, WIC_FLAGS_NONE, &metadata, scratchImg);

	if (FAILED(result)) {
		return result;
	}

	const Image *img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = dx_base_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&tex_data_.texture_buffer));
	if (FAILED(result)) {
		return result;
	}

	// テクスチャバッファにデータ転送
	result = tex_data_.texture_buffer->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		return result;
	}

	// サイズを取得
	descriptor_handle_increment_size_ = dx_base_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// シェーダリソースビュー作成
	tex_data_.cpu_desc_handle_SRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptor_handle_increment_size_);
	tex_data_.gpu_desc_handle_SRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptor_heap_->GetGPUDescriptorHandleForHeapStart(), 0, descriptor_handle_increment_size_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = tex_data_.texture_buffer->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dx_base_->GetDevice()->CreateShaderResourceView(tex_data_.texture_buffer.Get(), //ビューと関連付けるバッファ
													&srvDesc, //テクスチャ設定情報
													tex_data_.cpu_desc_handle_SRV
	);

	return true;
}

void Model::LoadMaterial(const std::string &directoryPath, const std::string &filename) {
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// 一行ずつ読み込む
	string line;
	while (getline(file, line)) {
		// 一行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin());	// 先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {
			// マテリアル名読み込み
			line_stream >> material_data_.name_;
		}

		// 先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material_data_.ambient.x;
			line_stream >> material_data_.ambient.y;
			line_stream >> material_data_.ambient.z;
		}

		// 先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material_data_.diffuse.x;
			line_stream >> material_data_.diffuse.y;
			line_stream >> material_data_.diffuse.z;
		}

		// 先頭文字列がKsならスペキュラー色
		if (key == "Ks") {
			line_stream >> material_data_.specular.x;
			line_stream >> material_data_.specular.y;
			line_stream >> material_data_.specular.z;
		}

		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			// テクスチャのファイル名読み込み
			line_stream >> material_data_.texture_file_name;
			// テクスチャ読み込み
			LoadTexture(directoryPath, material_data_.texture_file_name);
		}
	}
}
