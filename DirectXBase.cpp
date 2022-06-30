#include "DirectXBase.h"
#include "Win32App.h"
#include <dxgi.h>
#include <string>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

DirectXBase::DirectXBase() {
	device_ = nullptr;
	dxgiFactory = nullptr;
	tmpAdapter = nullptr;
	cmdAllocator = nullptr;
	cmdList = nullptr;
	cmdQueue = nullptr;
	swapchain = nullptr;
	backBuffers.resize(2);
	rtvHeaps = nullptr;
	depthBuffer = nullptr;
	dsvHeap = nullptr;
	fence = nullptr;
	fenceVal = 0;
}

void DirectXBase::Initialize() {
	ActivateDebugLayer();
	GenerateDXGIFactory();
	GenerateDevice();
	GenerateCommandList();
	GenerateCommandQueue();
	GenerateSwapChain();
	GenerateRenderTargetView();
	GenerateDepthBuffer();
	GenerateDepthStencilView();
	GenerateFence();
}

void DirectXBase::ActivateDebugLayer() {
#ifdef _DEBUG
	// デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif
}

void DirectXBase::GenerateDXGIFactory() {
	HRESULT result;
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
}

void DirectXBase::GenerateDevice() {
	HRESULT result;
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		adapters.push_back(tmpAdapter);
	}

	for (int i = 0; i < adapters.size(); i++) {
		// アダプターの情報を取得
		adapters[i]->GetDesc1(&adesc);

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		// アダプター名
		wstring strDesc = adesc.Description;

		// Intel UHD Graphicsを回避
		if (strDesc.find(L"Intel") == wstring::npos) {
			// 採用
			tmpAdapter = adapters[i];
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};


	for (int i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device_));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}

void DirectXBase::GenerateCommandAllocator() {
	HRESULT result;
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
}

void DirectXBase::GenerateCommandList() {
	HRESULT result;
	GenerateCommandAllocator();

	result = device_->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&cmdList));
}

void DirectXBase::GenerateCommandQueue() {
	device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXBase::GenerateSwapChain() {
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 色情報の書式
	swapchainDesc.SampleDesc.Count = 1;								// マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				// バックバッファ用
	swapchainDesc.BufferCount = 2;									// バッファ数を２に設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェインの作成
	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		Win32App::GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapchain1.As(&swapchain);
}

void DirectXBase::GenerateDescriptorHeap() {
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;					// 裏表の２つ
	device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
}

void DirectXBase::GenerateRenderTargetView() {
	HRESULT result;
	GenerateDescriptorHeap();

	// 裏表の2つ分について
	//vector<ID3D12Resource *> backBuffers(2);
	for (int i = 0; i < 2; i++) {
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		handle.ptr += i * device_->GetDescriptorHandleIncrementSize(heapDesc.Type);
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}
}

void DirectXBase::GenerateDepthBuffer() {
	HRESULT result;
	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Win32App::GetWindowWidth(),
		Win32App::GetWindowHeight(),
		1,
		0,
		1,
		0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	// 深度バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	// 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);
}

void DirectXBase::GenerateDepthStencilView() {
	HRESULT result;
	// 深度ビュー用デスクリプタヒープ作成
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// 深度ビュー作成
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DirectXBase::GenerateFence() {
	HRESULT result;
	result = device_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}
