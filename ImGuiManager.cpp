#include "ImGuiManager.h"
#include "Window.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

ComPtr<ID3D12DescriptorHeap> ImGuiManager::heap_for_imgui_;

void ImGuiManager::Initialize(ID3D12Device *device) {
	// ImGuiの初期化
	heap_for_imgui_ = CreateDescriptorHeapForImgui(device);

	if (ImGui::CreateContext() == nullptr) {
		assert(0);
	}

	// ImGuiのWindows用の初期化
	bool bln_result = ImGui_ImplWin32_Init(Window::GetHwnd());
	if (!bln_result) {
		assert(0);
	}

	// ImGuiのDirectX12用の初期化
	bln_result = ImGui_ImplDX12_Init(
		device,
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		GetHeapForImgui().Get(),
		GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(),
		GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::PreDraw() {
	// ImGui描画前処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ImGuiウィンドウの定義
	ImGui::Begin("Debug Menu");
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
}

void ImGuiManager::Draw(ID3D12GraphicsCommandList *cmd_list) {
	ImGui::End();

	// ImGuiウィンドウの描画
	ImGui::Render();
	cmd_list->SetDescriptorHeaps(
		1,
		GetHeapForImgui().GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmd_list);
}

ComPtr<ID3D12DescriptorHeap> ImGuiManager::CreateDescriptorHeapForImgui(ID3D12Device *device) {
	ComPtr<ID3D12DescriptorHeap> ret;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	device->CreateDescriptorHeap(
		&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));

	return ret;
}

ComPtr<ID3D12DescriptorHeap> ImGuiManager::GetHeapForImgui() {
	return heap_for_imgui_;
}
