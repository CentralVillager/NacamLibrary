#pragma once
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx12.h"
#include <wrl.h>
#include "DirectXBase.h"
#include <DirectXMath.h>

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

public:

	inline static void SliderUINT(const char *label, UINT &args, int min, int max) {

		int temp = args;
		if (temp < 0) { temp = 0; }
		ImGui::SliderInt(label, &temp, min, max);
		args = temp;
	}

	inline static void SliderFloat3(const char *label, DirectX::XMFLOAT3 &args, float min, float max) {

		float temp[3] = { args.x, args.y, args.z, };

		ImGui::SliderFloat3(label, temp, min, max);

		args.x = temp[0];
		args.y = temp[1];
		args.z = temp[2];
	}

	inline static void DragFloat3(const char *label, DirectX::XMFLOAT3 &args, float v_speed, float min, float max) {

		float temp[3] = { args.x, args.y, args.z, };

		ImGui::DragFloat3(label, temp, v_speed, min, max);

		args.x = temp[0];
		args.y = temp[1];
		args.z = temp[2];
	}
};
