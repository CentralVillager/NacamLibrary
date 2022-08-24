#include "../DrawProc/DrawProc.h"
#include "../DirectXBase/DirectXBase.h"
#include "../Win32App/Win32App.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

UINT DrawProc::back_buffer_index_;
float DrawProc::clear_color_[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
ComPtr<ID3D12GraphicsCommandList> DrawProc::command_list_;

void DrawProc::StaticInitialize()
{
	command_list_ = DirectXBase::GetInstance()->GetCommandList();
}

void DrawProc::PreDraw(DirectXBase *dx_base)
{
	// バックバッファの番号を取得（２つなので0番か1番）
	back_buffer_index_ = dx_base->GetSwapchain()->GetCurrentBackBufferIndex();

	// リソースバリアで書き込み可能に変更
	// 表示状態から描画状態に変更
	command_list_->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 描画先指定
	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx_base->GetRtvHeaps()->GetCPUDescriptorHandleForHeapStart(),
		back_buffer_index_,
		dx_base->GetDevice()->GetDescriptorHandleIncrementSize(dx_base->GetHeapDesc().Type)
	);

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dx_base->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
	dx_base->GetCommandList()->OMSetRenderTargets(
		1,
		&handle,
		false,
		&dsvH);

	// 画面クリア
	dx_base->GetCommandList()->ClearRenderTargetView(handle, clear_color_, 0, nullptr);

	// 深度バッファのクリア
	dx_base->GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートの設定コマンド
	dx_base->GetCommandList()->RSSetViewports(
		1,
		&CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			(float)(Win32App::GetWindowWidth()),
			(float)(Win32App::GetWindowHeight())));

	// シザー矩形の設定コマンド
	dx_base->GetCommandList()->RSSetScissorRects(
		1,
		&CD3DX12_RECT(
			(long)(0.0f),
			(long)(0.0f),
			Win32App::GetWindowWidth(),
			Win32App::GetWindowHeight()));
}

void DrawProc::PostDraw(DirectXBase *dx_base)
{
	// ４．描画コマンド 終了
	// ５．リソースバリアを戻す
	// 描画状態から表示状態に変更
	dx_base->GetCommandList()->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(dx_base->GetBackBuffers()[back_buffer_index_].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));

	// 命令のクローズ
	dx_base->GetCommandList()->Close();

	// コマンドリストの実行
	ID3D12CommandList *command_lists[] = { dx_base->GetCommandList().Get() };	// コマンドリストの配列
	dx_base->GetCommandQueue()->ExecuteCommandLists(1, command_lists);

	// バッファをフリップ
	dx_base->GetSwapchain()->Present(1, 0);

	// コマンドキューの実行完了を待つ
	dx_base->GetCommandQueue()->Signal(dx_base->GetFence().Get(), dx_base->IncrementFenceVal());

	if (dx_base->GetFence()->GetCompletedValue() != dx_base->GetFenceVal())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);

		dx_base->GetFence()->SetEventOnCompletion(dx_base->GetFenceVal(), event);

		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// キューをクリア
	dx_base->GetCommandAllocator()->Reset();

	// 再びコマンドリストを貯める準備
	dx_base->GetCommandList()->Reset(dx_base->GetCommandAllocator().Get(), nullptr);
}

void DrawProc::ClearDepthBuffer()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(DirectXBase::GetInstance()->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	DirectXBase::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
