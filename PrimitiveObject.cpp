#include "PrimitiveObject.h"
#include <d3dcompiler.h>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
using namespace DirectX;

ComPtr<ID3D12Device> PrimitiveObject::device_;
UINT PrimitiveObject::descriptor_handle_increment_size_;
ComPtr<ID3D12GraphicsCommandList> PrimitiveObject::cmd_list_;
ComPtr<ID3D12RootSignature> PrimitiveObject::root_signature_;
ComPtr<ID3D12PipelineState> PrimitiveObject::pipeline_state_;
ComPtr<ID3D12DescriptorHeap> PrimitiveObject::desc_heap_;
Camera *PrimitiveObject::cam_ptr_;

void PrimitiveObject::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList) {

	device_ = device;
	cmd_list_ = cmdList;

	InitializeDescriptorHeap();

	InitializeGraphicsPipeline();
}

bool PrimitiveObject::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&desc_heap_));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptor_handle_increment_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool PrimitiveObject::InitializeGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PrimitiveVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PrimitivePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&root_signature_));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = root_signature_.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_state_));

	if (FAILED(result)) {
		return result;
	}

	return true;
}

void PrimitiveObject::PreDraw() {

	// パイプラインステートの設定
	cmd_list_->SetPipelineState(pipeline_state_.Get());

	// ルートシグネチャの設定
	cmd_list_->SetGraphicsRootSignature(root_signature_.Get());

	// プリミティブ形状を設定
	cmd_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void PrimitiveObject::Initialize(XMFLOAT3 start, XMFLOAT3 end) {
	// nullptrチェック
	assert(device_);

	HRESULT result;
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrixConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_)
	);

	// 頂点データを反映----------------
	VertexData vertex{};
	vertex.pos = start;
	vertices_.emplace_back(vertex);
	indices_.emplace_back(0);

	vertex.pos = end;
	vertices_.emplace_back(vertex);
	indices_.emplace_back(1);
	//----------------------------------

	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices_.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	// 頂点バッファ生成
	result = device_->CreateCommittedResource(
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
	result = device_->CreateCommittedResource(
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

void PrimitiveObject::Finalize() {
}

void PrimitiveObject::Update() {
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ワールド行列の合成
	mat_world_ = XMMatrixIdentity();	// 変形をリセット
	mat_world_ *= matScale;				// ワールド行列にスケーリングを反映
	mat_world_ *= matRot;				// ワールド行列に回転を反映
	mat_world_ *= matTrans;				// ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent_ != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		mat_world_ *= parent_->mat_world_;
	}

	// 定数バッファへデータ転送
	const XMMATRIX &mat_view_ = cam_ptr_->GetMatView();
	const XMMATRIX &mat_projection = cam_ptr_->GetMatProjection();

	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&matrix_const_map));
	matrix_const_map->mat = mat_world_ * mat_view_ * mat_projection;	// 行列の合成
	matrix_const_buffer_->Unmap(0, nullptr);

	/*vertices_[0].pos.z += 0.001f;
	vertices_[1].pos.z += 0.001f;*/

	// 頂点バッファへのデータ転送
	VertexData *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vert_map);
		vertex_buffer_->Unmap(0, nullptr);
	}
}

void PrimitiveObject::Draw() {
}

void PrimitiveObject::DrawBox(XMFLOAT2 left_upper, XMFLOAT2 right_downer, int color) {
}

void PrimitiveObject::DrawLine() {
	// nullptrチェック
	assert(device_);
	assert(PrimitiveObject::cmd_list_);

	// 頂点バッファの設定
	cmd_list_->IASetVertexBuffers(0, 1, &vertex_buffer_view_);

	// インデックスバッファの設定
	cmd_list_->IASetIndexBuffer(&index_buffer_view_);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { desc_heap_.Get() };
	cmd_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmd_list_->SetGraphicsRootConstantBufferView(0, matrix_const_buffer_->GetGPUVirtualAddress());

	// 描画コマンド
	cmd_list_->DrawInstanced(2, 1, 0, 0);
}
