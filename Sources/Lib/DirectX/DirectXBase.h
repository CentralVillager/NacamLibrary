#pragma once
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include "../Win32App/Win32App.h"
#include "../../Lib/Singleton/Singleton.h"

using namespace std;

class DirectXBase final : public Singleton<DirectXBase>
{
	friend Singleton<DirectXBase>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12Debug> debug_controller_0_;
	ComPtr<ID3D12Debug1> debug_controller_1_;
	ComPtr<IDXGIFactory6> dxgi_factory_;
	vector<ComPtr<IDXGIAdapter1>> adapters_;
	DXGI_ADAPTER_DESC1 adapter_desc_;
	ComPtr<IDXGIAdapter1> tmp_adapter_;
	D3D_FEATURE_LEVEL feature_level_;
	ComPtr<ID3D12CommandAllocator> command_allocator_;
	ComPtr<ID3D12GraphicsCommandList> command_list_;
	D3D12_COMMAND_QUEUE_DESC command_queue_desc_{};
	ComPtr<ID3D12CommandQueue> command_queue_;
	DXGI_SWAP_CHAIN_DESC1 swapchain_desc_{};
	ComPtr<IDXGISwapChain4> swapchain_;
	ComPtr<IDXGISwapChain1> swapchain1_;
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc_{};
	vector<ComPtr<ID3D12Resource>> back_buffers_;
	ComPtr<ID3D12DescriptorHeap> rtv_heaps_;
	D3D12_RESOURCE_DESC depth_res_desc_{};
	D3D12_HEAP_PROPERTIES depth_heap_prop_{};
	D3D12_CLEAR_VALUE depth_clear_value_{};
	ComPtr<ID3D12Resource> depth_buffer_;
	D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc_{};
	ComPtr<ID3D12DescriptorHeap> dsv_heap_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc_{};
	ComPtr<ID3D12Fence> fence_;
	UINT fence_val_;

private:

	DirectXBase();

public:

	inline const ComPtr<ID3D12Device> &GetDevice() { return device_; }
	inline const ComPtr<ID3D12CommandAllocator> &GetCommandAllocator() { return command_allocator_; }
	inline const ComPtr<ID3D12GraphicsCommandList> &GetCommandList() { return command_list_; }
	inline const ComPtr<ID3D12CommandQueue> &GetCommandQueue() { return command_queue_; }
	inline const ComPtr<IDXGISwapChain4> &GetSwapchain() { return swapchain_; }
	inline const D3D12_DESCRIPTOR_HEAP_DESC &GetHeapDesc() { return heap_desc_; }
	inline const vector<ComPtr<ID3D12Resource>> &GetBackBuffers() { return back_buffers_; }
	inline const ComPtr<ID3D12DescriptorHeap> &GetRtvHeaps() { return rtv_heaps_; }
	inline const ComPtr<ID3D12DescriptorHeap> &GetDsvHeap() { return dsv_heap_; }
	inline const ComPtr<ID3D12Fence> &GetFence() { return fence_; }
	inline const UINT &GetFenceVal() { return fence_val_; }

	inline UINT IncrementFenceVal() { return ++fence_val_; }

	void Initialize();

	void ActivateDebugLayer();					// デバッグレイヤーをオンに
	void GenerateDXGIFactory();					// DXGIファクトリーの生成
	void GenerateDevice();						// デバイスの生成
	void GenerateCommandAllocator();			// コマンドアロケータを生成
	void GenerateCommandList();					// コマンドリストを生成
	void GenerateCommandQueue();				// 標準設定でコマンドキューを生成
	void GenerateSwapChain();					// 各種設定をしてスワップチェーンを生成
	void GenerateDescriptorHeap();				// 各種設定をしてデスクリプタヒープを生成
	void GenerateRenderTargetView();			// レンダーターゲットビューの生成
	void GenerateDepthBuffer();					// 深度バッファの生成
	void GenerateDepthStencilView();			// 深度ビューの生成
	void GenerateFence();						// フェンスの生成
};
