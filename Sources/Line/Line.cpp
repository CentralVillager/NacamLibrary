#include "../Line/Line.h"
#include "../DirectXBase/DirectXBase.h"
#include "../Debug/ImGuiManager.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> Line::device_ = nullptr;
ComPtr<ID3D12GraphicsCommandList> Line::command_list_ = nullptr;
ComPtr<ID3D12DescriptorHeap> Line::desc_heap_ = nullptr;
UINT Line::desc_heap_size_;
Camera *Line::cam_ptr_;

void Line::StaticInitialize()
{
	device_ = DirectXBase::GetInstance()->GetDevice().Get();
	command_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&desc_heap_));
	desc_heap_->SetName(L"LineDescriptorHeap");

	// デスクリプタサイズを取得
	desc_heap_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Line::CreateVertexBuffer()
{
	HRESULT result;

	Vertex line_vertices[] = {

		{ { -1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 0.0f, 0.0f } }
	};

	const UINT vertex_buffer_size = sizeof(line_vertices);

	// 頂点バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertex_buffer_size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	vertex_buffer_->SetName(L"LineVertexBuffer");

	Vertex *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result))
	{
		std::memcpy(vert_map, line_vertices, sizeof(Vertex) * 2);
		vertex_buffer_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの生成
	vb_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vb_view_.StrideInBytes = sizeof(Vertex);
	vb_view_.SizeInBytes = sizeof(line_vertices);
}

void Line::CreateConstantBuffer()
{
	HRESULT result;

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(MatrixConstBufferData)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_));
	matrix_const_buffer_->SetName(L"LineMatrixConstBuffer");
}

void Line::Initialize()
{
	CreateVertexBuffer();
	CreateConstantBuffer();
}

void Line::Finalize()
{}

void Line::Update()
{
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

	const XMMATRIX &mat_view_ = cam_ptr_->GetMatView();
	const XMMATRIX &mat_projection = cam_ptr_->GetMatProjection();

	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&matrix_const_map));
	matrix_const_map->mat = mat_world_ * mat_view_ * mat_projection;	// 行列の合成
	matrix_const_buffer_->Unmap(0, nullptr);

	// 頂点バッファへのデータ転送
	Vertex *vert_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result))
	{
		std::copy(vertices_data_.begin(), vertices_data_.end(), vert_map);
		vertex_buffer_->Unmap(0, nullptr);
	}
}

void Line::Draw()
{
	// 頂点バッファの設定
	command_list_->IASetVertexBuffers(0, 1, &vb_view_);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { desc_heap_.Get() };
	command_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	command_list_->SetGraphicsRootConstantBufferView(0, matrix_const_buffer_->GetGPUVirtualAddress());

	// 描画コマンド
	command_list_->DrawInstanced((UINT)(2), 1, 0, 0);
}

void Line::DebugDraw()
{
	ImGuiManager::DragFloat3("pos", vertices_data_[0].pos, 0.1f, -10.0f, 10.0f);
}
