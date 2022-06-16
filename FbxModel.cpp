#include "FbxModel.h"

void FbxModel::CreateBuffers(ID3D12Device *device) {
	HRESULT result;

	// 頂点データ全体のサイズ
	UINT size_vb = static_cast<UINT>(sizeof(VertexData) * vertices_.size());

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size_vb),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vert_buff_));

	// 頂点バッファへのデータ転送
	VertexData *vert_map = nullptr;
	result = vert_buff_->Map(0, nullptr, reinterpret_cast<void **>(&vert_map));
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vert_map);
		vert_buff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vb_view.BufferLocation = vert_buff_->GetGPUVirtualAddress();
	vb_view.SizeInBytes = size_vb;
	vb_view.StrideInBytes = sizeof(vertices_[0]);

	// 頂点インデックス全体のサイズ
	UINT size_ib = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size_ib),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buff_));

	// インデックスバッファへのデータ転送
	unsigned short *index_map = nullptr;
	result = index_buff_->Map(0, nullptr, reinterpret_cast<void **>(&index_map));
	if (SUCCEEDED(result)) {
		std::copy(indices_.begin(), indices_.end(), index_map);
		index_buff_->Unmap(0, nullptr);
	}

	//インデックスバッファビューの作成
	ib_view.BufferLocation = index_buff_->GetGPUVirtualAddress();
	ib_view.Format = DXGI_FORMAT_R16_UINT;
	ib_view.SizeInBytes = size_ib;

	// テクスチャ画像データ
	const DirectX::Image *img = scratch_img.GetImage(0, 0, 0);
	assert(img);

	// リソース設定
	CD3DX12_RESOURCE_DESC tex_res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
								 D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&tex_res_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tex_buff_)
	);

	// テクスチャバッファにデータ転送
	result = tex_buff_->WriteToSubresource(
		0,
		nullptr,							// 全領域へコピー
		img->pixels,						// 元データアドレス
		static_cast<UINT>(img->rowPitch),	// 1ラインサイズ
		static_cast<UINT>(img->slicePitch)	// 1枚サイズ
	);

	// SRV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
	desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc_heap_desc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(&desc_heap_desc, IID_PPV_ARGS(&desc_heap_srv));

	// シェーダーリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
	D3D12_RESOURCE_DESC res_desc = tex_buff_->GetDesc();

	srv_desc.Format = res_desc.Format;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(tex_buff_.Get(), &srv_desc, desc_heap_srv->GetCPUDescriptorHandleForHeapStart());
}

FbxModel::~FbxModel() {
	// FBXシーンの解放
	//fbx_scene_->Destroy();
}

void FbxModel::Draw(ID3D12GraphicsCommandList *cmd_list) {
	// 頂点バッファをセット
	cmd_list->IASetVertexBuffers(0, 1, &vb_view);
	// インデックスバッファをセット
	cmd_list->IASetIndexBuffer(&ib_view);

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap *pp_heaps[] = { desc_heap_srv.Get() };
	cmd_list->SetDescriptorHeaps(_countof(pp_heaps), pp_heaps);
	// シェーダリソースビューをセット
	cmd_list->SetGraphicsRootDescriptorTable(1, desc_heap_srv->GetGPUDescriptorHandleForHeapStart());

	// 描画コマンド
	cmd_list->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);
}
