#include "NcmSprite.h"
#include <cassert>
#include <DirectXTex.h>
#include "../Win32App/Win32App.h"
#include "../Converter/TextureConverter.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> NcmSprite::device_ = nullptr;
UINT NcmSprite::desc_handle_incre_size_;
ComPtr<ID3D12GraphicsCommandList> NcmSprite::cmd_list_ = nullptr;
XMMATRIX NcmSprite::mat_projection_;
ComPtr<ID3D12DescriptorHeap> NcmSprite::desc_heap_;
ComPtr<ID3D12Resource> NcmSprite::tex_buff_[srv_count_];
UINT NcmSprite::handle_counter_ = 0;
std::vector<NcmSprite::DrawData> NcmSprite::sprite_hub_;

void NcmSprite::TermSprite()
{
	sprite_hub_.clear();
	handle_counter_ = 0;
}

void NcmSprite::StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list)
{
	HRESULT result;

	device_ = device;
	cmd_list_ = cmd_list;

	desc_handle_incre_size_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 射影行列計算
	mat_projection_ = XMMatrixOrthographicOffCenterLH(
		0.0f,
		float(Win32App::SIZE_.x), float(Win32App::SIZE_.y),
		0.0f, 0.0f, 1.0f);

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = srv_count_;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&desc_heap_));//生成

	assert(SUCCEEDED(result));
}

ncm_thandle NcmSprite::LoadTex(const string &path)
{
	// 文字形式を変更
	wstring w_path = TextureConverter::ConvertMultiByteStringToWideString(path);

	handle_counter_++;
	int32_t count_for_array = handle_counter_ - 1;

	// nullptrチェック
	assert(device_);

	HRESULT result;

	TexMetadata meta_data{};
	ScratchImage scratch_img{};

	wstring r = TextureConverter::ReturnFileExtention(w_path);

	if (w_path.find(L"dds") != wstring::npos)
	{
		// DDSテクスチャのロード
		result = LoadFromDDSFile(w_path.c_str(), DDS_FLAGS_NONE, &meta_data, scratch_img);
	}
	else
	{
		// WICテクスチャのロード
		result = LoadFromWICFile(w_path.c_str(), WIC_FLAGS_NONE, &meta_data, scratch_img);
	}
	assert(SUCCEEDED(result));

	const Image *img = scratch_img.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC tex_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		meta_data.format,
		meta_data.width,
		(UINT)meta_data.height,
		(UINT16)meta_data.arraySize,
		(UINT16)meta_data.mipLevels
	);

	// テクスチャ用バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&tex_res_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&tex_buff_[count_for_array]));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	result = tex_buff_[count_for_array]->WriteToSubresource(
		0,
		nullptr,				// 全領域へコピー
		img->pixels,			// 元データアドレス
		(UINT)img->rowPitch,	// 1ラインサイズ
		(UINT)img->slicePitch	// 1枚サイズ
	);
	assert(SUCCEEDED(result));

	// シェーダリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{}; // 設定構造体
	D3D12_RESOURCE_DESC res_desc = tex_buff_[count_for_array]->GetDesc();

	srv_desc.Format = res_desc.Format;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srv_desc.Texture2D.MipLevels = 1;

	GenerateDrawData(count_for_array, w_path.c_str());

	device_->CreateShaderResourceView(tex_buff_[count_for_array].Get(), //ビューと関連付けるバッファ
		&srv_desc, //テクスチャ設定情報
		sprite_hub_[count_for_array].cpu_desc_handle_srv_
	);

	return count_for_array;
}

void NcmSprite::DrawTex(const ncm_thandle handle)
{
	// ワールド行列の更新
	sprite_hub_[handle].mat_world_ = XMMatrixIdentity();
	sprite_hub_[handle].mat_world_ *= XMMatrixRotationZ(XMConvertToRadians(sprite_hub_[handle].rot_.z));
	sprite_hub_[handle].mat_world_ *= XMMatrixTranslation(sprite_hub_[handle].pos_.x, sprite_hub_[handle].pos_.y, 0.0f);

	// 定数バッファにデータ転送
	ConstBufferData *const_map = nullptr;
	HRESULT result = sprite_hub_[handle].const_buffer_->Map(0, nullptr, (void **)&const_map);
	if (SUCCEEDED(result))
	{
		const_map->color = sprite_hub_[handle].color_;
		const_map->mat = sprite_hub_[handle].mat_world_ * mat_projection_;
		sprite_hub_[handle].const_buffer_->Unmap(0, nullptr);
	}

	// 頂点バッファの設定
	cmd_list_->IASetVertexBuffers(0, 1, &sprite_hub_[handle].vb_view_);
	ID3D12DescriptorHeap *pp_heaps[] = { desc_heap_.Get() };
	// デスクリプタヒープをセット
	cmd_list_->SetDescriptorHeaps(_countof(pp_heaps), pp_heaps);
	// 定数バッファビューをセット
	cmd_list_->SetGraphicsRootConstantBufferView(0, sprite_hub_[handle].const_buffer_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd_list_->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_->GetGPUDescriptorHandleForHeapStart(), handle, desc_handle_incre_size_));
	// 描画コマンド
	cmd_list_->DrawInstanced(4, 1, 0, 0);
}

void NcmSprite::DrawTex(const ncm_thandle handle, const XMFLOAT2 &pos)
{
	// Transを設定
	SetPos(handle, pos);

	// 描画
	DrawTex(handle);
}

void NcmSprite::GenerateDrawData(const ncm_thandle handle, const wchar_t *filename)
{
	HRESULT result = S_FALSE;

	// 要素を構築
	sprite_hub_.emplace_back();

	// デスクリプタハンドルを取得
	sprite_hub_.back().cpu_desc_handle_srv_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(desc_heap_->GetCPUDescriptorHandleForHeapStart(), handle, desc_handle_incre_size_);
	sprite_hub_.back().gpu_desc_handle_srv_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(desc_heap_->GetGPUDescriptorHandleForHeapStart(), handle, desc_handle_incre_size_);

	// 頂点バッファ生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vert_num_),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite_hub_.back().vertex_buffer_));
	sprite_hub_.back().vertex_buffer_->SetName(L"SpriteVertexBuffer");
	assert(SUCCEEDED(result));

	// テクスチャ情報取得
	D3D12_RESOURCE_DESC res_desc = tex_buff_[handle]->GetDesc();
	// スプライトのサイズをテクスチャのサイズに設定
	sprite_hub_.back().size_ = { (float)res_desc.Width, (float)res_desc.Height };
	sprite_hub_.back().tex_size_ = sprite_hub_.back().size_;
	TransferVertices(&sprite_hub_.back());

	// 頂点バッファビューの作成
	sprite_hub_.back().vb_view_.BufferLocation = sprite_hub_.back().vertex_buffer_->GetGPUVirtualAddress();
	sprite_hub_.back().vb_view_.SizeInBytes = sizeof(VertexPosUv) * 4;
	sprite_hub_.back().vb_view_.StrideInBytes = sizeof(VertexPosUv);
	assert(SUCCEEDED(result));

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite_hub_.back().const_buffer_));
	sprite_hub_.back().const_buffer_->SetName(L"SpriteConstantBuffer");
	assert(SUCCEEDED(result));

	// 定数バッファにデータ転送
	ConstBufferData *constMap = nullptr;
	result = sprite_hub_.back().const_buffer_->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = sprite_hub_.back().color_;
		constMap->mat = mat_projection_;
		sprite_hub_.back().const_buffer_->Unmap(0, nullptr);
	}

	sprite_hub_.back().scale_ = 1.0f;

	// 識別用に管理
	sprite_hub_.back().handle = handle;
	sprite_hub_.back().file_path = filename;
}

void NcmSprite::TransferVertices(DrawData *itr)
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - itr->anchorpoint_.x) * itr->size_.x;
	float right = (1.0f - itr->anchorpoint_.x) * itr->size_.x;
	float top = (0.0f - itr->anchorpoint_.y) * itr->size_.y;
	float bottom = (1.0f - itr->anchorpoint_.y) * itr->size_.y;
	if (itr->is_flip_x_)
	{	// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (itr->is_flip_y_)
	{	// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VertexPosUv vertices[vert_num_];

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	if (tex_buff_[itr->handle])
	{
		D3D12_RESOURCE_DESC res_desc = tex_buff_[itr->handle]->GetDesc();

		float tex_left = itr->tex_base_.x / res_desc.Width;
		float tex_right = (itr->tex_base_.x + itr->tex_size_.x) / res_desc.Width;
		float tex_top = itr->tex_base_.y / res_desc.Height;
		float tex_bottom = (itr->tex_base_.y + itr->tex_size_.y) / res_desc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom };	// 左下
		vertices[LT].uv = { tex_left,	tex_top };		// 左上
		vertices[RB].uv = { tex_right,	tex_bottom };	// 右下
		vertices[RT].uv = { tex_right,	tex_top };		// 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv *vert_map = nullptr;
	result = itr->vertex_buffer_->Map(0, nullptr, (void **)&vert_map);
	if (SUCCEEDED(result))
	{
		memcpy(vert_map, vertices, sizeof(vertices));
		itr->vertex_buffer_->Unmap(0, nullptr);
	}
}
