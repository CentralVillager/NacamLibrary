#include "IndirectObject3d.h"
#include "DirectXBase.h"
#include <d3dcompiler.h>
#include "KeyboardInput.h"
#include "Utility.h"
#include "ImGuiManager.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

const UINT IndirectObject3d::command_size_per_frame_ = all_particle_num_ * sizeof(IndirectCommand);
ComPtr<ID3D12RootSignature> IndirectObject3d::root_signature_ = nullptr;
ComPtr<ID3D12PipelineState> IndirectObject3d::pipeline_state_ = nullptr;
ComPtr<ID3D12GraphicsCommandList> IndirectObject3d::command_list_ = nullptr;
Camera *IndirectObject3d::camera_ = nullptr;

IndirectObject3d::IndirectObject3d() {

	const_buffer_data_.resize(all_particle_num_);
	matrix_const_buffer_data_.resize(all_particle_num_);
	material_const_buffer_data_.resize(all_particle_num_);

	if (!model_) {

		// モデルデータの生成
		model_ = make_unique<Model>();
		model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

IndirectObject3d::~IndirectObject3d() {
}

void IndirectObject3d::CreateVertexBuffer() {

	HRESULT result;

	Vertex triangle_vertices[] = {

		{{0.0f, 0.5f, 0.0f}, {0, 0, 0}, {0, 0}},
		{{0.5f, -0.5f, 0.0f}, {0, 0, 0}, {0, 0}},
		{{-0.5f, -0.5f, 0.0f}, {0, 0, 0}, {0, 0}},
	};

	const UINT vertex_buffer_size = sizeof(triangle_vertices);

	// 頂点バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertex_buffer_size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	vertex_buffer_->SetName(L"VertexBuffer");

	D3D12_SUBRESOURCE_DATA vertex_data = {};
	vertex_data.pData = reinterpret_cast<UINT8 *>(triangle_vertices);
	vertex_data.RowPitch = vertex_buffer_size;
	vertex_data.SlicePitch = vertex_data.RowPitch;

	Vertex *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {
		std::memcpy(vert_map, triangle_vertices, sizeof(Vertex) * 3);
		vertex_buffer_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの生成
	vb_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vb_view_.StrideInBytes = sizeof(Vertex);
	vb_view_.SizeInBytes = sizeof(triangle_vertices);
}

void IndirectObject3d::CreateDescriptorHeap() {

	HRESULT result = S_FALSE;

	// デスクリプタヒープを作成
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
	//desc_heap_desc.NumDescriptors = 3;
	desc_heap_desc.NumDescriptors = 3 * frame_count_;
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	device_->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&descriptor_heap_));
	descriptor_heap_->SetName(L"DescriptorHeap");

	assert(SUCCEEDED(result));

	// デスクリプタサイズを取得
	descriptor_heap_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.Buffer.NumElements = all_particle_num_;
	srv_desc.Buffer.StructureByteStride = sizeof(MatrixConstBufferData) + sizeof(Model::MaterialConstBufferData);
	srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	CD3DX12_CPU_DESCRIPTOR_HANDLE cbv_srv_handle(descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptor_heap_size_);
	for (UINT frame = 0; frame < frame_count_; frame++) {

		//srv_desc.Buffer.FirstElement = all_particle_num_;
		srv_desc.Buffer.FirstElement = frame * all_particle_num_;
		device_->CreateShaderResourceView(material_const_buffer_.Get(), &srv_desc, cbv_srv_handle);
		cbv_srv_handle.Offset(3, descriptor_heap_size_);
	}
}

void IndirectObject3d::CreatePipelineState() {

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vs_blob;
	ComPtr<ID3DBlob> ps_blob;
	ComPtr<ID3DBlob> error_blob;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/IndirectVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vs_blob, &error_blob);
	if (FAILED(result)) {

		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(),
			error_blob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/IndirectPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&ps_blob, &error_blob);
	if (FAILED(result)) {

		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(),
			error_blob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());

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
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &error_blob);
	// ルートシグネチャの生成
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&root_signature_));

	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = root_signature_.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline_state_));

	assert(SUCCEEDED(result));
}

void IndirectObject3d::CreateGraphicsPipeline() {
}

void IndirectObject3d::CreateConstantBuffer() {

	HRESULT result;

	//const UINT const_buffer_data_size = sizeof(ConstBufferData);
	const UINT const_buffer_data_size = resource_count_ * sizeof(ConstBufferData);

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(MatrixConstBufferData) * all_particle_num_),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_));
	matrix_const_buffer_->SetName(L"MatrixConstBuffer");

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(MatrixConstBufferData) * all_particle_num_),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_const_buffer_));
	matrix_const_buffer_->SetName(L"MaterialConstBuffer");

	for (UINT i = 0; i < all_particle_num_; i++) {

		material_const_buffer_data_[i].ambient = model_->GetMaterialData().ambient;
		material_const_buffer_data_[i].diffuse = model_->GetMaterialData().diffuse;
		material_const_buffer_data_[i].specular = model_->GetMaterialData().specular;
		material_const_buffer_data_[i].alpha = model_->GetMaterialData().alpha;

		matrix_const_buffer_data_[i].mat = XMMatrixIdentity();	// 仮代入

		const_buffer_data_[i].position_ = { 0, 0, 0 };
		const_buffer_data_[i].velocity_ = { 0, 0, 0 };
		const_buffer_data_[i].color_ = { 0, 0, 0 };
	}

	TransferConstantBafferData();
}

void IndirectObject3d::CreateCommandSignature() {

	HRESULT result = S_FALSE;

	D3D12_INDIRECT_ARGUMENT_DESC argument_desc[3]{};
	argument_desc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
	argument_desc[0].ConstantBufferView.RootParameterIndex = 0;
	argument_desc[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
	argument_desc[1].ConstantBufferView.RootParameterIndex = 1;
	argument_desc[2].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	D3D12_COMMAND_SIGNATURE_DESC command_signature_desc = {};
	command_signature_desc.pArgumentDescs = argument_desc;
	command_signature_desc.NumArgumentDescs = _countof(argument_desc);
	command_signature_desc.ByteStride = sizeof(IndirectCommand);

	result = device_->CreateCommandSignature(&command_signature_desc, root_signature_.Get(), IID_PPV_ARGS(&command_signature_));
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
	//commands.resize(all_particle_num_);
	commands.resize(resource_count_);
	//const UINT command_buffer_size = command_size_per_frame_;
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
	command_buffer_->SetName(L"CommandBuffer");

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(command_buffer_size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&command_buffer_upload_)
	);
	command_buffer_upload_->SetName(L"CommandBufferUpload");

	D3D12_GPU_VIRTUAL_ADDRESS matrix_gpu_address = matrix_const_buffer_->GetGPUVirtualAddress();
	D3D12_GPU_VIRTUAL_ADDRESS material_gpu_address = material_const_buffer_->GetGPUVirtualAddress();
	UINT command_index = 0;
	UINT vertex_count = (UINT)(model_->GetVertices().size());

	for (UINT frame = 0; frame < frame_count_; frame++) {
		for (UINT n = 0; n < all_particle_num_; n++) {

			commands[command_index].matrix_cbv_ = matrix_gpu_address;
			commands[command_index].material_cbv_ = material_gpu_address;

			// これらはDrawIndexedInstancedの引数と全く同じ
			/*commands[command_index].draw_arguments_.IndexCountPerInstance = vertex_count;
			commands[command_index].draw_arguments_.InstanceCount = 1;
			commands[command_index].draw_arguments_.StartIndexLocation = 0;
			commands[command_index].draw_arguments_.BaseVertexLocation = 0;
			commands[command_index].draw_arguments_.StartInstanceLocation = 0;*/

			// DrawInstanced
			commands[command_index].draw_arguments_.VertexCountPerInstance = 3;
			commands[command_index].draw_arguments_.InstanceCount = 1;
			commands[command_index].draw_arguments_.StartVertexLocation = 0;
			commands[command_index].draw_arguments_.StartInstanceLocation = 0;

			command_index++;
			matrix_gpu_address += sizeof(MatrixConstBufferData);
			material_gpu_address += sizeof(Model::MaterialConstBufferData);
		}
	}

	// データを中間アップロードヒープにコピーし、その後、アップロードヒープからコマンドバッファへのコピーをスケジュールします。
	// アップロードヒープからコマンドバッファにコピーする。
	D3D12_SUBRESOURCE_DATA command_data = {};
	command_data.pData = reinterpret_cast<UINT8 *>(&commands[0]);
	command_data.RowPitch = command_buffer_size;
	command_data.SlicePitch = command_data.RowPitch;

	UpdateSubresources<1>(command_list_.Get(), command_buffer_.Get(), command_buffer_upload_.Get(), 0, 0, 1, &command_data);

	// コマンドバッファ用のSRVを生成
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.Buffer.NumElements = all_particle_num_;
	srv_desc.Buffer.StructureByteStride = sizeof(IndirectCommand);
	srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	CD3DX12_CPU_DESCRIPTOR_HANDLE commands_handle(descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), 1, descriptor_heap_size_);

	for (UINT frame = 0; frame < frame_count_; frame++) {

		//srv_desc.Buffer.FirstElement = all_particle_num_;
		srv_desc.Buffer.FirstElement = frame * all_particle_num_;
		device_->CreateShaderResourceView(command_buffer_.Get(), &srv_desc, commands_handle);
		commands_handle.Offset(3, descriptor_heap_size_);
	}
}

void IndirectObject3d::TransferConstantBafferData() {

	HRESULT result;

	Model::MaterialConstBufferData *material_const_map = nullptr;
	result = material_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&material_const_map));
	std::memcpy(material_const_map, &material_const_buffer_data_[0], material_const_buffer_data_.size() * sizeof(MatrixConstBufferData));
	material_const_buffer_->Unmap(0, nullptr);

	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&matrix_const_map));
	std::memcpy(matrix_const_map, &matrix_const_buffer_data_[0], matrix_const_buffer_data_.size() * sizeof(Model::MaterialConstBufferData));
	matrix_const_buffer_->Unmap(0, nullptr);
}

void IndirectObject3d::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList) {
}

void IndirectObject3d::PreDraw() {

	// パイプラインステートの設定
	command_list_->SetPipelineState(pipeline_state_.Get());
	// ルートシグネチャの設定
	command_list_->SetGraphicsRootSignature(root_signature_.Get());
	// プリミティブ形状を設定
	command_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void IndirectObject3d::Initialize() {

	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	command_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

	CreateVertexBuffer();
	CreateDescriptorHeap();
	CreatePipelineState();
	CreateConstantBuffer();
	CreateCommandSignature();
	CreateCommandBuffer();
}

void IndirectObject3d::Finalize() {
}

void IndirectObject3d::Update() {

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

	const XMMATRIX &mat_view_ = camera_->GetMatView();
	const XMMATRIX &mat_projection = camera_->GetMatProjection();

	if (is_push_) {

		for (UINT i = 0; i < all_particle_num_; i++) {
			position_ = NacamUtility::GenerateRandom({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f });
			matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);
			mat_world_ *= matTrans;
			matrix_const_buffer_data_[i].mat = mat_world_ * mat_view_ * mat_projection;
		}
	}

	TransferConstantBafferData();
}

void IndirectObject3d::Draw() {

	// 頂点バッファの設定
	//command_list_->IASetVertexBuffers(0, 1, &model_->GetVBView());
	command_list_->IASetVertexBuffers(0, 1, &vb_view_);
	// インデックスバッファの設定
	//command_list_->IASetIndexBuffer(&model_->GetIBView());

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { descriptor_heap_.Get() };
	//ID3D12DescriptorHeap *ppHeaps[] = { model_->GetDescriptorHeap().Get() };
	command_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	command_list_->SetGraphicsRootConstantBufferView(0, matrix_const_buffer_->GetGPUVirtualAddress());
	//command_list_->SetGraphicsRootConstantBufferView(1, material_const_buffer_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	//command_list_->SetGraphicsRootDescriptorTable(2, model_->GetGpuDescHandleSRV());
	// 描画コマンド
	//command_list_->DrawIndexedInstanced(static_cast<UINT>(model_->GetIndices().size()), 1, 0, 0, 0);
	//command_list_->DrawInstanced((UINT)(3), 1, 0, 0);

	D3D12_RESOURCE_BARRIER barrier[] = {

		CD3DX12_RESOURCE_BARRIER::Transition(
			command_buffer_.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT),
	};

	// リソースバリアを変更
	command_list_->ResourceBarrier(_countof(barrier), barrier);

	command_list_->ExecuteIndirect(
		command_signature_.Get(),
		all_particle_num_,
		command_buffer_.Get(),
		static_cast<UINT64>(command_size_per_frame_) * frame_index_,
		nullptr,
		0);

	barrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
	barrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

	// リソースバリアを変更
	command_list_->ResourceBarrier(_countof(barrier), barrier);
}

void IndirectObject3d::DebugDraw() {

	ImGui::Text("SPACE : SetPosition");
}
