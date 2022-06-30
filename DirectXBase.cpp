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
	// �f�o�b�O���C���[���I����
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
		// �A�_�v�^�[�̏����擾
		adapters[i]->GetDesc1(&adesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		// �A�_�v�^�[��
		wstring strDesc = adesc.Description;

		// Intel UHD Graphics�����
		if (strDesc.find(L"Intel") == wstring::npos) {
			// �̗p
			tmpAdapter = adapters[i];
			break;
		}
	}

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};


	for (int i = 0; i < _countof(levels); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device_));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
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
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// �F���̏���
	swapchainDesc.SampleDesc.Count = 1;								// �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				// �o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2;									// �o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// �t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�C���̍쐬
	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		Win32App::GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	// ��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapchain1.As(&swapchain);
}

void DirectXBase::GenerateDescriptorHeap() {
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// �����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2;					// ���\�̂Q��
	device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
}

void DirectXBase::GenerateRenderTargetView() {
	HRESULT result;
	GenerateDescriptorHeap();

	// ���\��2���ɂ���
	//vector<ID3D12Resource *> backBuffers(2);
	for (int i = 0; i < 2; i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		handle.ptr += i * device_->GetDescriptorHandleIncrementSize(heapDesc.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device_->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}
}

void DirectXBase::GenerateDepthBuffer() {
	HRESULT result;
	// �[�x�o�b�t�@���\�[�X�ݒ�
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

	// �[�x�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	// �[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);
}

void DirectXBase::GenerateDepthStencilView() {
	HRESULT result;
	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// �[�x�r���[�쐬
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
