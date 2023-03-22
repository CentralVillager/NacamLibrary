#include "../DirectX/DirectXBase.h"
#include "../Win32App/Win32App.h"
#include <dxgi.h>
#include <string>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

DirectXBase::DirectXBase()
{
	device_ = nullptr;
	dxgi_factory_ = nullptr;
	tmp_adapter_ = nullptr;
	command_allocator_ = nullptr;
	command_list_ = nullptr;
	command_queue_ = nullptr;
	swapchain_ = nullptr;
	back_buffers_.resize(2);
	rtv_heaps_ = nullptr;
	depth_buffer_ = nullptr;
	dsv_heap_ = nullptr;
	fence_ = nullptr;
	fence_val_ = 0;
}

void DirectXBase::Initialize()
{
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

void DirectXBase::ActivateDebugLayer()
{
#ifdef _DEBUG
	//if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller_)))) {

	//	// デバッグレイヤーをオン
	//	debug_controller_->EnableDebugLayer();
	//	debug_controller_->SetEnableGPUBasedValidation(TRUE);
	//}

	D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller_0_));
	debug_controller_0_->EnableDebugLayer();
	debug_controller_0_->QueryInterface(IID_PPV_ARGS(&debug_controller_1_));
	debug_controller_1_->SetEnableGPUBasedValidation(true);
#endif
}

void DirectXBase::GenerateDXGIFactory()
{
	HRESULT result;
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory_));
}

void DirectXBase::GenerateDevice()
{
	HRESULT result;

	for (int i = 0; dxgi_factory_->EnumAdapters1(i, &tmp_adapter_) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters_.push_back(tmp_adapter_);
	}

	for (int i = 0; i < adapters_.size(); i++)
	{
		// アダプターの情報を取得
		adapters_[i]->GetDesc1(&adapter_desc_);

		// ソフトウェアデバイスを回避
		if (adapter_desc_.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		// アダプター名
		wstring strDesc = adapter_desc_.Description;

		// Intel UHD Graphicsを回避
		if (strDesc.find(L"Intel") == wstring::npos)
		{
			// 採用
			tmp_adapter_ = adapters_[i];
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};


	for (int i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmp_adapter_.Get(), levels[i], IID_PPV_ARGS(&device_));
		device_->SetName(L"Device");

		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			feature_level_ = levels[i];
			break;
		}
	}
}

void DirectXBase::GenerateCommandAllocator()
{
	HRESULT result;

	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&command_allocator_));
	command_allocator_->SetName(L"CommandAllocater");
}

void DirectXBase::GenerateCommandList()
{
	HRESULT result;

	GenerateCommandAllocator();

	result = device_->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		command_allocator_.Get(),
		nullptr,
		IID_PPV_ARGS(&command_list_));
	command_list_->SetName(L"CommandList");
}

void DirectXBase::GenerateCommandQueue()
{
	device_->CreateCommandQueue(&command_queue_desc_, IID_PPV_ARGS(&command_queue_));
	command_queue_->SetName(L"CommandQueue");
}

void DirectXBase::GenerateSwapChain()
{
	swapchain_desc_.Width = 1280;
	swapchain_desc_.Height = 720;
	swapchain_desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 色情報の書式
	swapchain_desc_.SampleDesc.Count = 1;								// マルチサンプルしない
	swapchain_desc_.BufferUsage = DXGI_USAGE_BACK_BUFFER;				// バックバッファ用
	swapchain_desc_.BufferCount = 2;									// バッファ数を２に設定
	swapchain_desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// フリップ後は破棄
	swapchain_desc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェインの作成
	dxgi_factory_->CreateSwapChainForHwnd(
		command_queue_.Get(),
		Win32App::GetHwnd(),
		&swapchain_desc_,
		nullptr,
		nullptr,
		&swapchain1_);

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapchain1_.As(&swapchain_);
}

void DirectXBase::GenerateDescriptorHeap()
{
	heap_desc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// レンダーターゲットビュー
	heap_desc_.NumDescriptors = 2;					// 裏表の２つ
	device_->CreateDescriptorHeap(&heap_desc_, IID_PPV_ARGS(&rtv_heaps_));
	rtv_heaps_->SetName(L"RTVHeaps");
}

void DirectXBase::GenerateRenderTargetView()
{
	HRESULT result;

	GenerateDescriptorHeap();

	// 裏表の2つ分について
	//vector<ID3D12Resource *> backBuffers(2);

	for (int i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain_->GetBuffer(i, IID_PPV_ARGS(&back_buffers_[i]));

		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv_heaps_->GetCPUDescriptorHandleForHeapStart();

		// 裏か表かでアドレスがずれる
		handle.ptr += i * device_->GetDescriptorHandleIncrementSize(heap_desc_.Type);

		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(
			back_buffers_[i].Get(),
			nullptr,
			handle);
	}
}

void DirectXBase::GenerateDepthBuffer()
{
	HRESULT result;

	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Win32App::SIZE_.x,
		Win32App::SIZE_.y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	// 深度バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	// 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depth_buffer_)
	);
	depth_buffer_->SetName(L"DepthBuffer");
}

void DirectXBase::GenerateDepthStencilView()
{
	HRESULT result;

	// 深度ビュー用デスクリプタヒープ作成
	dsv_heap_desc_.NumDescriptors = 1;
	dsv_heap_desc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device_->CreateDescriptorHeap(&dsv_heap_desc_, IID_PPV_ARGS(&dsv_heap_));

	// 深度ビュー作成
	dsv_desc_.Format = DXGI_FORMAT_D32_FLOAT;
	dsv_desc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depth_buffer_.Get(),
		&dsv_desc_,
		dsv_heap_->GetCPUDescriptorHandleForHeapStart()
	);
}

void DirectXBase::GenerateFence()
{
	HRESULT result;
	result = device_->CreateFence(fence_val_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	fence_->SetName(L"FenceVal");
}
