#include "NcmPlatePoly.h"

using namespace DirectX;

Camera *NcmPlatePoly::cam_ptr_ = nullptr;

void NcmPlatePoly::Update(const ncm_thandle handle)
{
	HRESULT result;
	XMMATRIX mat_scale, mat_rot, mat_trans;

	// スケール、回転、平行移動行列の計算
	mat_scale = XMMatrixScaling(sprite_hub_[handle].scale_, sprite_hub_[handle].scale_, sprite_hub_[handle].scale_);
	mat_rot = XMMatrixIdentity();
	mat_rot *= XMMatrixRotationZ(XMConvertToRadians(sprite_hub_[handle].rot_.z));
	mat_rot *= XMMatrixRotationX(XMConvertToRadians(sprite_hub_[handle].rot_.x));
	mat_rot *= XMMatrixRotationY(XMConvertToRadians(sprite_hub_[handle].rot_.y));
	mat_trans = XMMatrixTranslation(sprite_hub_[handle].pos_.x, sprite_hub_[handle].pos_.y, sprite_hub_[handle].pos_.z);

	// ワールド行列の合成
	sprite_hub_[handle].mat_world_ = XMMatrixIdentity();	// 変形をリセット
	sprite_hub_[handle].mat_world_ *= mat_scale;			// ワールド行列にスケーリングを反映
	sprite_hub_[handle].mat_world_ *= mat_rot;				// ワールド行列に回転を反映
	sprite_hub_[handle].mat_world_ *= mat_trans;			// ワールド行列に平行移動を反映

	// 定数バッファへ転送
	ConstBufferData *const_map = nullptr;
	result = sprite_hub_[handle].const_buffer_->Map(0, nullptr, (void **)&const_map);
	const_map->mat = sprite_hub_[handle].mat_world_ * cam_ptr_->GetMatView() * cam_ptr_->GetMatProjection();
	const_map->mat_billboard = cam_ptr_->GetMatBillboard();
	sprite_hub_[handle].const_buffer_->Unmap(0, nullptr);
}

void NcmPlatePoly::Draw(const ncm_thandle handle)
{
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
