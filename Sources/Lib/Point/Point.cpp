#include "Point.h"
#include "../PreDraw/PreDraw.h"
#include <cassert>

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> Point::device_ = nullptr;
ComPtr<ID3D12GraphicsCommandList> Point::cmd_list_ = nullptr;
UINT Point::descriptor_handle_incre_size_ = 0;
ComPtr<ID3D12DescriptorHeap> Point::desc_heap_ = nullptr;
Camera *Point::camera_ = nullptr;

Point::Point() :
	cpu_desc_handle_srv_(),
	gpu_desc_handle_srv_(),
	vert_buff_(),
	const_buff_(),
	vb_view_(),
	mat_world_(),
	tex_handle_(),
	pos_(),
	rot_(),
	scale_(),
	color_()
{}

Point::~Point()
{}

void Point::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list)
{
	device_ = device;
	cmd_list_ = cmd_list;
}

void Point::Initialize()
{
	HRESULT result;

	// 初期化
	InitializeDescriptorHeap();
	CreateVertexBuffer();

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buff_));
	const_buff_->SetName(L"PointConstBuffer");
	assert(SUCCEEDED(result));

	pos_ = XMFLOAT3(0, 0, 0);
	rot_ = XMFLOAT3(0, 0, 0);
	scale_ = 1.0f;
	color_ = XMFLOAT3(1, 1, 1);
}

void Point::Update()
{
	HRESULT result;

	VertexPos *vert_map = nullptr;
	result = vert_buff_->Map(0, nullptr, (void **)&vert_map);
	vert_map->pos = pos_;
	vert_map->scale = scale_;
	vert_buff_->Unmap(0, nullptr);

	XMMATRIX mat_scale, mat_rot, mat_trans;

	// スケール、回転、平行移動行列の計算
	mat_scale = XMMatrixScaling(scale_, scale_, scale_);
	mat_rot = XMMatrixIdentity();
	mat_rot *= XMMatrixRotationZ(XMConvertToRadians(rot_.z));
	mat_rot *= XMMatrixRotationX(XMConvertToRadians(rot_.x));
	mat_rot *= XMMatrixRotationY(XMConvertToRadians(rot_.y));
	mat_trans = XMMatrixTranslation(pos_.x, pos_.y, pos_.z);

	// ワールド行列の合成
	mat_world_ = XMMatrixIdentity();	// 変形をリセット
	mat_world_ *= mat_scale;			// ワールド行列にスケーリングを反映
	mat_world_ *= mat_rot;				// ワールド行列に回転を反映
	mat_world_ *= mat_trans;			// ワールド行列に平行移動を反映

	// 定数バッファへ転送
	ConstBufferData *const_map = nullptr;
	result = const_buff_->Map(0, nullptr, (void **)&const_map);
	const_map->mat = camera_->GetMatView() * camera_->GetMatProjection();
	//const_map->mat = mat_world_ * camera_->GetMatView() * camera_->GetMatProjection();
	const_map->mat_billboard = camera_->GetMatBillboard();
	const_buff_->Unmap(0, nullptr);
}

void Point::Draw()
{
	// 頂点バッファの設定
	cmd_list_->IASetVertexBuffers(0, 1, &vb_view_);
	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { desc_heap_.Get() };
	cmd_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmd_list_->SetGraphicsRootConstantBufferView(0, const_buff_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd_list_->SetGraphicsRootDescriptorTable(1, NcmSprite::GetDrawData(tex_handle_).gpu_desc_handle_srv_);
	//cmd_list_->SetGraphicsRootDescriptorTable(1, gpu_desc_handle_srv_);
	// 描画コマンド
	cmd_list_->DrawInstanced(1, 1, 0, 0);
}

void Point::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープの生成	
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	desc_heap_desc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&desc_heap_));//生成
	desc_heap_->SetName(L"PointDescriptorHeap");
	assert(SUCCEEDED(result));

	// デスクリプタサイズを取得
	descriptor_handle_incre_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Point::CreateVertexBuffer()
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos)),
		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos) * vertex_count_),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vert_buff_));
	vert_buff_->SetName(L"PointVertexBuffer");
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	vb_view_.BufferLocation = vert_buff_->GetGPUVirtualAddress();
	vb_view_.SizeInBytes = sizeof(VertexPos) * vertex_count_;
	vb_view_.StrideInBytes = sizeof(VertexPos);
}
