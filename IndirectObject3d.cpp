#include "IndirectObject3d.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> IndirectObject3d::device_ = nullptr;
UINT IndirectObject3d::descriptor_handle_increment_size_ = 0;
ComPtr<ID3D12GraphicsCommandList> IndirectObject3d::cmd_list_ = nullptr;
ComPtr<ID3D12RootSignature> IndirectObject3d::rootsignature_;
ComPtr<ID3D12PipelineState> IndirectObject3d::pipelinestate_;
ComPtr<ID3D12DescriptorHeap> IndirectObject3d::desc_heap_;
Camera *IndirectObject3d::cam_ptr_ = nullptr;
const UINT IndirectObject3d::command_size_per_frame_ = all_particle_num_ * sizeof(IndirectCommand);
ComPtr<ID3D12Resource> IndirectObject3d::material_const_buffer_;
ComPtr<ID3D12Resource> IndirectObject3d::matrix_const_buffer_;
ComPtr<ID3D12Resource> IndirectObject3d::const_buffer_;

bool IndirectObject3d::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList) {

	// nullptrチェック
	assert(device);

	IndirectObject3d::device_ = device;
	IndirectObject3d::cmd_list_ = cmdList;

	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	// パイプライン初期化
	InitializeGraphicsPipeline();

	return true;
}

void IndirectObject3d::PreDraw() {

	// パイプラインステートの設定
	cmd_list_->SetPipelineState(pipelinestate_.Get());
	// ルートシグネチャの設定
	cmd_list_->SetGraphicsRootSignature(rootsignature_.Get());
	// プリミティブ形状を設定
	cmd_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool IndirectObject3d::InitializeDescriptorHeap() {

	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc_heap_desc.NumDescriptors = 3 * frame_count_;
	result = device_->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&desc_heap_));

	assert(SUCCEEDED(result));

	// デスクリプタサイズを取得
	descriptor_handle_increment_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool IndirectObject3d::InitializeGraphicsPipeline() {

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",	// シェーダファイル名
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
		L"Resources/shaders/BasicPS.hlsl",	// シェーダファイル名
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
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;	// 描画対象は2つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
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
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature_));

	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature_.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate_));

	assert(SUCCEEDED(result));

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrixConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_));

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Model::MaterialConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_const_buffer_));

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buffer_));

	return true;
}

void IndirectObject3d::CreateCommandSignature() {

	HRESULT result = S_FALSE;

	D3D12_INDIRECT_ARGUMENT_DESC argument_desc[2]{};
	argument_desc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
	argument_desc[0].ConstantBufferView.RootParameterIndex = 0;
	argument_desc[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	D3D12_COMMAND_SIGNATURE_DESC command_signature_desc = {};
	command_signature_desc.pArgumentDescs = argument_desc;
	command_signature_desc.NumArgumentDescs = _countof(argument_desc);
	command_signature_desc.ByteStride = sizeof(IndirectCommand);

	result = device_->CreateCommandSignature(&command_signature_desc, IndirectObject3d::rootsignature_.Get(), IID_PPV_ARGS(&command_signature_));
}

void IndirectObject3d::CreateCommandBuffer() {

	// 公式サンプルから引用----------------------------------------------------------------
	// 注意: ComPtr は CPU オブジェクトですが、これらのリソースは、
	// それらを参照するコマンドリストが GPU 上で実行を終了するまで、
	// これらのリソースはスコープ内にある必要があります。
	// このメソッドの最後に GPU をフラッシュし、リソースが早期に破壊されないようにします。
	// 時期尚早に破棄されないようにするため、このメソッドの最後に GPU をフラッシュします。
	ComPtr<ID3D12Resource> vertex_buffer_upload;
	ComPtr<ID3D12Resource> command_buffer_upload;

	HRESULT result;

	std::vector<IndirectCommand> commands;
	commands.resize(resource_count_);
	const UINT command_buffer_size = command_size_per_frame_ * frame_count_;

	D3D12_RESOURCE_DESC command_buffer_desc = CD3DX12_RESOURCE_DESC::Buffer(command_buffer_size);

	// コマンドバッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&command_buffer_desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&command_buffer_)
	);

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(command_buffer_size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&command_buffer_upload)
	);

	D3D12_GPU_VIRTUAL_ADDRESS gpu_address = const_buffer_->GetGPUVirtualAddress();
	UINT command_index = 0;

	for (UINT frame = 0; frame < frame_count_; frame++) {
		for (UINT n = 0; n < all_particle_num_; n++) {

			commands[command_index].cbv_ = gpu_address;
			commands[command_index].draw_arguments_.InstanceCount = 1;
			commands[command_index].draw_arguments_.StartInstanceLocation = 0;
			commands[command_index].draw_arguments_.StartVertexLocation = 0;
			commands[command_index].draw_arguments_.VertexCountPerInstance = model_ptr_->GetVertices().size();

			command_index++;
			gpu_address += sizeof(ConstBufferData);
		}
	}

	// データを中間アップロードヒープにコピーし、その後、アップロードヒープからコマンドバッファへのコピーをスケジュールします。
	// アップロードヒープからコマンドバッファにコピーする。
	D3D12_SUBRESOURCE_DATA command_data = {};
	command_data.pData = reinterpret_cast<UINT8 *>(&commands[0]);
	command_data.RowPitch = command_buffer_size;
	command_data.SlicePitch = command_data.RowPitch;

	/*UpdateSubresources<1>(cmd_list_.Get(), command_buffer_.Get(), command_buffer_upload.Get(), 0, 0, 1, &command_data);
	cmd_list_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(command_buffer_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));*/

	// コマンドバッファ用のSRVを生成
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.Buffer.NumElements = all_particle_num_;
	srv_desc.Buffer.StructureByteStride = sizeof(IndirectCommand);
	srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	CD3DX12_CPU_DESCRIPTOR_HANDLE commands_handle(desc_heap_->GetCPUDescriptorHandleForHeapStart(), 1, descriptor_handle_increment_size_);

	// offset何やってるかわからん なんなん
	for (UINT frame = 0; frame < frame_count_; frame++) {

		srv_desc.Buffer.FirstElement = frame * all_particle_num_;
		device_->CreateShaderResourceView(command_buffer_.Get(), &srv_desc, commands_handle);
		commands_handle.Offset(3, descriptor_handle_increment_size_);
	}
}

void IndirectObject3d::Initialize() {

	// nullptrチェック
	assert(device_);


	// スケールをセット
	float const scale_val = 1.0f;
	scale_ = { scale_val, scale_val, scale_val };

	CreateCommandSignature();
	CreateCommandBuffer();

}

void IndirectObject3d::Update() {

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
	Model::MaterialConstBufferData *material_const_map = nullptr;
	result = material_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&material_const_map));
	material_const_map->ambient = model_ptr_->GetMaterialData().ambient;
	material_const_map->diffuse = model_ptr_->GetMaterialData().diffuse;
	material_const_map->specular = model_ptr_->GetMaterialData().specular;
	material_const_map->alpha = model_ptr_->GetMaterialData().alpha;
	material_const_buffer_->Unmap(0, nullptr);

	const XMMATRIX &mat_view_ = cam_ptr_->GetMatView();
	const XMMATRIX &mat_projection = cam_ptr_->GetMatProjection();

	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&matrix_const_map));
	matrix_const_map->mat = mat_world_ * mat_view_ * mat_projection;	// 行列の合成
	matrix_const_buffer_->Unmap(0, nullptr);
}

void IndirectObject3d::Draw() {

	// nullptrチェック
	assert(device_);
	assert(IndirectObject3d::cmd_list_);

	// 頂点バッファの設定
	cmd_list_->IASetVertexBuffers(0, 1, &model_ptr_->GetVBView());
	// インデックスバッファの設定
	cmd_list_->IASetIndexBuffer(&model_ptr_->GetIBView());

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { model_ptr_->GetDescriptorHeap().Get() };
	cmd_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmd_list_->SetGraphicsRootConstantBufferView(0, matrix_const_buffer_->GetGPUVirtualAddress());
	cmd_list_->SetGraphicsRootConstantBufferView(1, material_const_buffer_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd_list_->SetGraphicsRootDescriptorTable(2, model_ptr_->GetGpuDescHandleSRV());
	// 描画コマンド
	cmd_list_->DrawIndexedInstanced(static_cast<UINT>(model_ptr_->GetIndices().size()), 1, 0, 0, 0);

	// コマンドシグネチャに登録した情報でcount_回描画する？
	cmd_list_->ExecuteIndirect(
		command_signature_.Get(),
		all_particle_num_,
		command_buffer_.Get(),
		command_size_per_frame_ * frame_index_,
		nullptr,
		0);

	frame_index_++;
}

const IndirectObject3d IndirectObject3d::operator=(const IndirectObject3d &rhs) {
	return *this;
}
