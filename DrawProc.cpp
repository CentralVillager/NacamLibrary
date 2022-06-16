#include "DrawProc.h"
#include "DirectXBase.h"
#include "Window.h"

UINT DrawProc::back_buffer_index_;
float DrawProc::clear_color_[4] = { 0.1f, 0.1f, 0.1f, 0.0f };

void DrawProc::PreDraw(DirectXBase *dx_base) {

	// バックバッファの番号を取得（２つなので0番か1番）
	back_buffer_index_ = dx_base->GetSwapchain()->GetCurrentBackBufferIndex();

	// リソースバリアで書き込み可能に変更
	// 表示状態から描画状態に変更
	dx_base->GetCmdList()->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		)
	);

	// 描画先指定
	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx_base->GetRtvHeaps()->GetCPUDescriptorHandleForHeapStart(),
		back_buffer_index_,
		dx_base->GetDevice()->GetDescriptorHandleIncrementSize(dx_base->GetHeapDesc().Type)
	);

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dx_base->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
	dx_base->GetCmdList()->OMSetRenderTargets(
		1,
		&handle,
		false,
		&dsvH
	);

	// 画面クリア
	dx_base->GetCmdList()->ClearRenderTargetView(handle, clear_color_, 0, nullptr);

	// 深度バッファのクリア
	dx_base->GetCmdList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートの設定コマンド
	dx_base->GetCmdList()->RSSetViewports(
		1,
		&CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			Window::GetWindowWidth(),
			Window::GetWindowHeight()
		)
	);

	// シザー矩形の設定コマンド
	dx_base->GetCmdList()->RSSetScissorRects(
		1,
		&CD3DX12_RECT(
			0.0f,
			0.0f,
			Window::GetWindowWidth(),
			Window::GetWindowHeight()
		)
	);

	// プリミティブ形状の設定コマンド
	//dx_init->getCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DrawProc::PostDraw(DirectXBase *dx_base) {

	// ４．描画コマンド 終了
	// ５．リソースバリアを戻す
	// 描画状態から表示状態に変更
	dx_base->GetCmdList()->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		)
	);

	// 命令のクローズ
	dx_base->GetCmdList()->Close();

	// コマンドリストの実行
	ID3D12CommandList *cmdLists[] = { dx_base->GetCmdList().Get() };	// コマンドリストの配列
	dx_base->GetCmdQueue()->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ
	dx_base->GetSwapchain()->Present(1, 0);

	// コマンドキューの実行完了を待つ
	dx_base->GetCmdQueue()->Signal(dx_base->GetFence().Get(), dx_base->incrementFenceVal());

	if (dx_base->GetFence()->GetCompletedValue() != dx_base->GetFenceVal()) {

		HANDLE event = CreateEvent(nullptr, false, false, nullptr);

		dx_base->GetFence()->SetEventOnCompletion(dx_base->GetFenceVal(), event);

		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// キューをクリア
	dx_base->GetCmdAllocator()->Reset();

	// 再びコマンドリストを貯める準備
	dx_base->GetCmdList()->Reset(dx_base->GetCmdAllocator().Get(), nullptr);
}

void DrawProc::ClearDepthBuffer() {

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(DirectXBase::GetInstance()->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	DirectXBase::GetInstance()->GetCmdList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
