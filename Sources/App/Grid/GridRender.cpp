#include "../Grid/GridRender.h"
#include "../../Lib/Utility/NcmUtil.h"

using namespace Microsoft::WRL;

ID3D12Device *GridRender::device_ = nullptr;
UINT GridRender::desc_incre_size_ = 0;
ID3D12GraphicsCommandList *GridRender::cmd_list_ = nullptr;
ComPtr<ID3D12DescriptorHeap> GridRender::desc_heap_;
ComPtr<ID3D12Resource> GridRender::vert_buff_;
D3D12_VERTEX_BUFFER_VIEW GridRender::vb_view_{};
std::array<GridRender::VertexData, GridRender::VERTEX_COUNT_> GridRender::vertices_;
Camera *GridRender::cam_ptr_ = nullptr;

GridRender::GridRender() :
	grid_(),
	all_vert_(),
	mat_const_buff_(),
	draw_vert_size_(),
	origin_pos_(),
	z_size_(),
	grid_num_(),
	offset_()
{}

GridRender::~GridRender()
{}

void GridRender::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list)
{
	device_ = device;
	cmd_list_ = cmd_list;

	InitializeDescriptorHeap();
	CreateVertexBuffer();
}

void GridRender::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	desc_heap_desc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&desc_heap_));//生成
	assert(SUCCEEDED(result));

	// デスクリプタサイズを取得
	desc_incre_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void GridRender::CreateVertexBuffer()
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices_)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vert_buff_));
	vert_buff_->SetName(L"GridRenderVertexBuffer");
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexData *vert_map = nullptr;
	result = vert_buff_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result))
	{
		memcpy(vert_map, &vertices_, sizeof(vertices_));
		vert_buff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vb_view_.BufferLocation = vert_buff_->GetGPUVirtualAddress();
	vb_view_.SizeInBytes = sizeof(vertices_);
	vb_view_.StrideInBytes = sizeof(vertices_[0]);
}

void GridRender::InitializeConstBuffer()
{
	HRESULT result;

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mat_const_buff_));
	mat_const_buff_->SetName(L"GridRenderConstantBuffer");
}

void GridRender::UpdateVertBuffer()
{
	// 頂点バッファへデータ転送
	VertexData *vert_map = nullptr;
	HRESULT result = vert_buff_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	//if (SUCCEEDED(result))
	//{
	//	// パーティクルの情報を1つずつ反映
	//	for (auto itr = args.begin(); itr != args.end(); itr++)
	//	{
	//		// 座標
	//		vert_map->pos = itr->pos;
	//		// 次の座標へ
	//		vert_map++;
	//	}
	//}

	// 全グリッドに対して
	//for (auto &i : grid_)
	//{
	//	// 始点と終点の位置をマップ
	//	for (int j = 0; j < 2; j++)
	//	{
	//		vert_map->pos = i.GetVertData()[j].pos;
	//		vert_map++;
	//	}
	//}
	vert_buff_->Unmap(0, nullptr);

	// 要素数（頂点数）を取得
	//draw_vert_size_ = (UINT)(std::distance(args.begin(), args.end()));
}

void GridRender::UpdateConstBuffer()
{
	// 定数バッファへデータを転送
	MatConstBufferData *const_map = nullptr;
	HRESULT result = mat_const_buff_->Map(0, nullptr, (void **)&const_map);
	const_map->mat = cam_ptr_->GetMatView() * cam_ptr_->GetMatProjection();
	mat_const_buff_->Unmap(0, nullptr);
}

const void GridRender::SetZPos(float pos)
{
	using enum VertDesc;

	origin_pos_.z = pos;

	for (auto &i : grid_)
	{
		float pos_z = i.GetPosition().z;
		pos_z += pos;
		i.SetPosition(XMFLOAT3(0, 0, pos_z));
		origin_pos_.z += pos;
	}

	// x軸に対してz軸方向に展開
	//float x_pos = (origin_pos_.x - (grid_num_ / 2) * offset_) / 2;
	//float z_pos = (origin_pos_.z + (grid_num_ / 2) * offset_) / 2;
	//for (UINT i = 0; i < grid_.size() / 2; i++)
	//{
	//	grid_[i].SetVertPos(XMFLOAT3(offset_ * i + x_pos, origin_pos_.y, -z_pos), (UINT)(Start));
	//	grid_[i].SetVertPos(XMFLOAT3(offset_ * i + x_pos, origin_pos_.y, z_pos), (UINT)(End));
	//}

	//// z軸に対してx軸方向に展開
	//z_pos = (origin_pos_.z - (grid_num_ / 2) * offset_) / 2;
	//for (UINT i = 0; i < grid_.size() / 2; i++)
	//{
	//	grid_[i + grid_.size() / 2].SetVertPos(XMFLOAT3(-x_pos, origin_pos_.y, offset_ * i + z_pos), (UINT)(Start));
	//	grid_[i + grid_.size() / 2].SetVertPos(XMFLOAT3(x_pos, origin_pos_.y, offset_ * i + z_pos), (UINT)(End));
	//}
}

void GridRender::Initialize(const UINT grid_num, const UINT offset, const XMFLOAT3 origin_pos)
{
	using enum VertDesc;

	grid_num_ = grid_num;
	offset_ = offset;

	// 定数バッファの初期化
	InitializeConstBuffer();

	// カメラをセット
	Line::SetCamera(cam_ptr_);

	// +2 しないとグリッドの終端が中途半端に終わります
	grid_.resize(grid_num_ + 2);

	for (auto &i : grid_)
	{
		i.Initialize();
	}

	// 基準位置を取得
	origin_pos_ = origin_pos;

	// x軸に対してz軸方向に展開
	float x_pos = (origin_pos.x - (grid_num_ / 2) * offset_) / 2;
	float z_pos = (origin_pos.z + (grid_num_ / 2) * offset_) / 2;
	for (UINT i = 0; i < grid_.size() / 2; i++)
	{
		grid_[i].SetVertPos(XMFLOAT3(offset_ * i + x_pos, origin_pos.y, -z_pos), (UINT)(Start));
		grid_[i].SetVertPos(XMFLOAT3(offset_ * i + x_pos, origin_pos.y, z_pos), (UINT)(End));
	}

	z_size_ = (float)(grid_.size() / 2) * offset_;

	// z軸に対してx軸方向に展開
	z_pos = (origin_pos.z - (grid_num_ / 2) * offset_) / 2;
	for (UINT i = 0; i < grid_.size() / 2; i++)
	{
		grid_[i + grid_.size() / 2].SetVertPos(XMFLOAT3(-x_pos, origin_pos.y, offset_ * i + z_pos), (UINT)(Start));
		grid_[i + grid_.size() / 2].SetVertPos(XMFLOAT3(x_pos, origin_pos.y, offset_ * i + z_pos), (UINT)(End));
	}
}

void GridRender::Update(float move_speed)
{
	for (auto &i : grid_)
	{
		i.Update();

		float pos_z = i.GetPosition().z;
		pos_z -= move_speed;
		i.SetPosition(XMFLOAT3(0, 0, pos_z));
		origin_pos_.z -= move_speed;
	}

	/*UpdateVertBuffer();
	UpdateConstBuffer();*/
}

void GridRender::Draw()
{
	for (auto &i : grid_)
	{
		i.Draw();
	}

	// nullptrチェック
	//assert(cmd_list_);

	//// 頂点バッファの設定
	//cmd_list_->IASetVertexBuffers(0, 1, &vb_view_);

	//// デスクリプタヒープの配列
	//ID3D12DescriptorHeap *pp_heaps[] = { desc_heap_.Get() };
	//cmd_list_->SetDescriptorHeaps(_countof(pp_heaps), pp_heaps);

	//// 定数バッファビューをセット
	//cmd_list_->SetGraphicsRootConstantBufferView(0, mat_const_buff_->GetGPUVirtualAddress());

	//// 描画コマンド
	//cmd_list_->DrawInstanced(draw_vert_size_, 1, 0, 0);
}

void GridRender::DebugDraw()
{}

void GridRender::MoveMinusZ()
{
	for (auto &i : grid_)
	{
		float pos_z = i.GetPosition().z;
		pos_z -= 0.5f;
		i.SetPosition(XMFLOAT3(0, 0, pos_z));
		origin_pos_.z -= 0.5f;
	}
}
