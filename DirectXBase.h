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

	void ActivateDebugLayer();					// �f�o�b�O���C���[���I����
	void GenerateDXGIFactory();					// DXGI�t�@�N�g���[�̐���
	void GenerateDevice();						// �f�o�C�X�̐���
	void GenerateCommandAllocator();			// �R�}���h�A���P�[�^�𐶐�
	void GenerateCommandList();					// �R�}���h���X�g�𐶐�
	void GenerateCommandQueue();				// �W���ݒ�ŃR�}���h�L���[�𐶐�
	void GenerateSwapChain();					// �e��ݒ�����ăX���b�v�`�F�[���𐶐�
	void GenerateDescriptorHeap();				// �e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
	void GenerateRenderTargetView();			// �����_�[�^�[�Q�b�g�r���[�̐���
	void GenerateDepthBuffer();					// �[�x�o�b�t�@�̐���
	void GenerateDepthStencilView();			// �[�x�r���[�̐���
	void GenerateFence();						// �t�F���X�̐���
};
