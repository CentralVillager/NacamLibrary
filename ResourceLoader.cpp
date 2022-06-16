//#include "ResourceLoader.h"
//#include <cassert>
//#include <d3dcompiler.h>
//#include <DirectXTex.h>
//
//#pragma comment(lib, "d3dcompiler.lib")
//
//using namespace DirectX;
//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//
//DirectXBase *ResourceLoader::dx_base_ = DirectXBase::GetInstance();
//ComPtr<ID3D12Device> ResourceLoader::device_ = DirectXBase::GetInstance()->GetDevice();
//
//unsigned short ResourceLoader::LoadGraph(const wchar_t *filename) {
//
//	Sprite sprite = {};
//
//	HRESULT result;
//	// WICテクスチャのロード
//	TexMetadata metadata{};
//	ScratchImage scratch_img{};
//
//	result = LoadFromWICFile(
//		filename, WIC_FLAGS_NONE,
//		&metadata, scratch_img);
//	if (FAILED(result)) {
//		assert(0);
//		return false;
//	}
//
//	const Image *img = scratch_img.GetImage(0, 0, 0); // 生データ抽出
//
//	// リソース設定
//	CD3DX12_RESOURCE_DESC texres_desc = CD3DX12_RESOURCE_DESC::Tex2D(
//		metadata.format,
//		metadata.width,
//		(UINT)metadata.height,
//		(UINT16)metadata.arraySize,
//		(UINT16)metadata.mipLevels
//	);
//
//	// テクスチャ用バッファの生成
//	result = device_->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
//		D3D12_HEAP_FLAG_NONE,
//		&texres_desc,
//		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
//		nullptr,
//		IID_PPV_ARGS(&tex_buff[texnumber]));
//	if (FAILED(result)) {
//		assert(0);
//		return false;
//	}
//
//	// テクスチャバッファにデータ転送
//	result = tex_buff[texnumber]->WriteToSubresource(
//		0,
//		nullptr, // 全領域へコピー
//		img->pixels,    // 元データアドレス
//		(UINT)img->rowPitch,  // 1ラインサイズ
//		(UINT)img->slicePitch // 1枚サイズ
//	);
//	if (FAILED(result)) {
//		assert(0);
//		return false;
//	}
//
//	// シェーダリソースビュー作成
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
//	D3D12_RESOURCE_DESC resDesc = tex_buff[texnumber]->GetDesc();
//
//	srvDesc.Format = resDesc.Format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
//	srvDesc.Texture2D.MipLevels = 1;
//
//	device_->CreateShaderResourceView(tex_buff[texnumber].Get(), //ビューと関連付けるバッファ
//									  &srvDesc, //テクスチャ設定情報
//									  CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber, descriptorHandleIncrementSize)
//	);
//
//	return true;
//}
//
//unsigned short ResourceLoader::LoadObj(const wchar_t *filename) {
//	return 0;
//}
