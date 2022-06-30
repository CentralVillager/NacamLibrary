#pragma once
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include "Win32App.h"
#include "Singleton.h"

using namespace std;

class DirectXBase final : public Singleton<DirectXBase> {
	friend Singleton<DirectXBase>;

public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	DirectXBase();

	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12Debug> debugController;
	ComPtr<IDXGIFactory6> dxgiFactory;
	vector<ComPtr<IDXGIAdapter1>> adapters;
	DXGI_ADAPTER_DESC1 adesc;
	ComPtr<IDXGIAdapter1> tmpAdapter;
	D3D_FEATURE_LEVEL featureLevel;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	ComPtr<ID3D12CommandQueue> cmdQueue;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<IDXGISwapChain1> swapchain1;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	D3D12_RESOURCE_DESC depthResDesc{};
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	D3D12_CLEAR_VALUE depthClearValue{};
	ComPtr<ID3D12Resource> depthBuffer;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	ComPtr<ID3D12Fence> fence;
	UINT fenceVal;

public:
	DirectXBase(const DirectXBase &obj) = delete;
	DirectXBase &operator=(const DirectXBase &obj) = delete;

	const ComPtr<ID3D12Device> &GetDevice() { return device_; }
	const ComPtr<ID3D12CommandAllocator> &GetCmdAllocator() { return cmdAllocator; }
	const ComPtr<ID3D12GraphicsCommandList> &GetCmdList() { return cmdList; }
	const ComPtr<ID3D12CommandQueue> &GetCmdQueue() { return cmdQueue; }
	const ComPtr<IDXGISwapChain4> &GetSwapchain() { return swapchain; }
	const D3D12_DESCRIPTOR_HEAP_DESC &GetHeapDesc() { return heapDesc; }
	const vector<ComPtr<ID3D12Resource>> &GetBackBuffers() { return backBuffers; }
	const ComPtr<ID3D12DescriptorHeap> &GetRtvHeaps() { return rtvHeaps; }
	const ComPtr<ID3D12DescriptorHeap> &GetDsvHeap() { return dsvHeap; }
	const ComPtr<ID3D12Fence> &GetFence() { return fence; }
	const UINT &GetFenceVal() { return fenceVal; }

	UINT incrementFenceVal() { return ++fenceVal; }

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
