#pragma once
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx12.h"
#include <wrl.h>
#include "DirectXBase.h"

class ImGuiManager {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	static ComPtr<ID3D12DescriptorHeap> heap_for_imgui_;

public:
	static void Initialize(ID3D12Device *device);
	static void PreDraw();
	static void Draw(ID3D12GraphicsCommandList *cmd_list);

	// imgui用のデスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui(ID3D12Device *device);
	// imgui用のヒープアクセサー
	static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();
};

