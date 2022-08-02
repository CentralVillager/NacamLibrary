#include "../Object3d/Object3d.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "../PreDraw/PreDraw.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ComPtr<ID3D12Device> Object3d::device = nullptr;
UINT Object3d::descriptorHandleIncrementSize = 0;
ComPtr<ID3D12GraphicsCommandList> Object3d::cmd_list_ = nullptr;
ComPtr<ID3D12RootSignature> Object3d::rootsignature;
ComPtr<ID3D12PipelineState> Object3d::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Object3d::descHeap;
Camera *Object3d::cam_ptr_ = nullptr;

bool Object3d::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList) {

	// nullptrチェック
	assert(device);

	Object3d::device = device;
	Object3d::cmd_list_ = cmdList;

	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	return true;
}

void Object3d::PreDraw() {

	// パイプラインステートの設定
	//cmd_list_->SetPipelineState(pipelinestate.Get());

	//// ルートシグネチャの設定
	//cmd_list_->SetGraphicsRootSignature(rootsignature.Get());

	//// プリミティブ形状を設定
	//cmd_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 仮置き
	PreDraw::PreRender(PipelineName::Object3d);
}

bool Object3d::InitializeDescriptorHeap() {

	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

void Object3d::Initialize() {

	assert(device);

	HRESULT result;
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrixConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_));

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Model::MaterialConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_const_buffer_));

	position_ = XMFLOAT3(0, 0, 0);
	rotation_ = XMFLOAT3(0, 0, 0);
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void Object3d::Update() {

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

void Object3d::Draw() {

	// nullptrチェック
	assert(device);
	assert(Object3d::cmd_list_);

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
}

const Object3d Object3d::operator=(const Object3d &rhs) {

	return *this;
}
