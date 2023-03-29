#include "../DebugUtill/NcmImGui.h"
#include "../Utility/NcmUtil.h"
#include "../../Lib/Win32App/Win32App.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

ComPtr<ID3D12DescriptorHeap> NcmImGui::heap_for_imgui_;

void NcmImGui::Initialize(ID3D12Device *device)
{
	// ImGuiの初期化
	heap_for_imgui_ = CreateDescriptorHeapForImgui(device);

	IMGUI_CHECKVERSION();
	if (ImGui::CreateContext() == nullptr)
	{
		assert(0);
	}

	// ImGuiのWinApp用の初期化
	bool bln_result = ImGui_ImplWin32_Init(Win32App::GetHwnd());
	if (!bln_result)
	{
		assert(0);
	}

	// ImGuiのDirectX12用の初期化
	bln_result = ImGui_ImplDX12_Init(
		device,
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		heap_for_imgui_.Get(),
		heap_for_imgui_->GetCPUDescriptorHandleForHeapStart(),
		heap_for_imgui_->GetGPUDescriptorHandleForHeapStart());

	// フォントの読み込み
	ImGuiIO &io = ImGui::GetIO();
	const float size_pixels = 13.5f;
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/UbuntuMono-R.ttf", size_pixels);

	NcmStyle();
}

void NcmImGui::PreDraw()
{
	static bool is_show_about = false;
	static bool is_show_demo = false;
	static bool is_show_metrics = false;
	static bool is_show_editor = false;

	// ImGui描画前処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags flag = 0;
	flag |= ImGuiWindowFlags_AlwaysAutoResize;	// 自動で枠サイズを変更

	// ImGuiウィンドウの定義
	ImGui::Begin("Debug Menu", 0, flag);

	ImGui::Text("fps : %f", ImGui::GetIO().Framerate);
	SelectStyle();

	ImGui::Checkbox("ShowAboutWindow", &is_show_about);
	if (is_show_about) { ImGui::ShowAboutWindow(); }

	ImGui::Checkbox("ShowDemoWindow", &is_show_demo);
	if (is_show_demo) { ImGui::ShowDemoWindow(); }

	ImGui::Checkbox("ShowMetricsWindow", &is_show_metrics);
	if (is_show_metrics) { ImGui::ShowMetricsWindow(&is_show_metrics); }

	ImGui::Checkbox("ShowStyleEditor", &is_show_editor);
	if (is_show_editor)
	{
		ImGui::Begin("StyleEditor");
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	ImGui::Separator();
}

void NcmImGui::Draw(ID3D12GraphicsCommandList *cmd_list)
{
	ImGui::End();

	// ImGuiウィンドウの描画
	ImGui::Render();

	cmd_list->SetDescriptorHeaps(
		1, heap_for_imgui_.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmd_list);
}

void NcmImGui::SelectStyle()
{
	static int style_idx = -1;
	if (ImGui::Combo("Style", &style_idx, "Dark\0Light\0Classic\0Ncm\0Ncm2\0"))
	{
		switch (style_idx)
		{
		case 0: ImGui::StyleColorsDark(); break;
		case 1: ImGui::StyleColorsLight(); break;
		case 2: ImGui::StyleColorsClassic(); break;
		case 3: NcmStyle(); break;
		case 4: NcmStyle2(); break;
		}
	}
}

void NcmImGui::NcmStyle()
{
	// 詳細設定
	ImGuiStyle *style = &ImGui::GetStyle();
	style->Alpha = 1.0f;
	style->AntiAliasedLines = false;
	style->WindowBorderSize = (1.0f);
	style->ChildBorderSize = (1.0f);
	style->PopupBorderSize = (1.0f);
	style->FrameBorderSize = (1.0f);
	style->WindowRounding = (0);
	style->ChildRounding = (0);
	style->FrameRounding = (0);
	style->PopupRounding = (0);
	style->GrabRounding = (0);
	style->TabRounding = (0);
	style->ScrollbarRounding = (0);

	ImVec4 *colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 0.63f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.86f, 0.43f, 0.00f, 0.71f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.86f, 0.24f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.39f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.78f, 0.78f, 0.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.39f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.86f, 0.43f, 0.00f, 0.24f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.86f, 0.43f, 0.00f, 0.35f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.63f, 0.71f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.63f, 0.71f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.27f, 0.27f, 0.27f, 0.78f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.86f, 0.24f, 0.00f, 0.24f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.41f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.86f, 0.43f, 0.00f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.86f, 0.43f, 0.00f, 0.59f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.86f, 0.24f, 0.00f, 0.24f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.51f, 0.51f, 0.51f, 0.39f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


}

void NcmImGui::NcmStyle2()
{
	ImVec4 *colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.55f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 1.00f, 0.00f, 0.66f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.55f, 1.00f, 0.00f, 0.51f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.55f, 1.00f, 0.00f, 0.12f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.39f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.78f, 0.78f, 0.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.55f, 1.00f, 0.00f, 0.39f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.79f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.55f, 1.00f, 0.00f, 0.16f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 1.00f, 0.00f, 0.35f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.63f, 0.71f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.27f, 0.27f, 0.27f, 0.78f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.55f, 1.00f, 0.00f, 0.27f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.41f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.51f, 1.00f, 0.00f, 0.78f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.55f, 1.00f, 0.00f, 0.59f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.55f, 1.00f, 0.00f, 0.24f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.51f, 0.51f, 0.51f, 0.39f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void NcmImGui::Term()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ComPtr<ID3D12DescriptorHeap> NcmImGui::CreateDescriptorHeapForImgui(ID3D12Device *device)
{
	ComPtr<ID3D12DescriptorHeap> ret;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;

	device->CreateDescriptorHeap(
		&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));

	return ret;
}
