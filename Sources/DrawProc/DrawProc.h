#pragma once
#include <Windows.h>
#include "../Singleton/Singleton.h"
#include <wrl.h>
#include <d3d12.h>
#include "../DirectXBase/DirectXBase.h"

class DrawProc : public Singleton<DrawProc> {
	friend Singleton<DrawProc>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	// 背景色
	static float clear_color_[4];

	// バックバッファ番号
	static UINT back_buffer_index_;

	static ComPtr<ID3D12GraphicsCommandList> command_list_;

public:

	static void StaticInitialize();

	/// <summary>
	/// バックバッファ準備
	/// </summary>
	/// <param name="dx_base"></param>
	static void PreDraw(DirectXBase *dx_base);

	/// <summary>
	/// バックバッファ描画
	/// </summary>
	/// <param name="dx_base"></param>
	static void PostDraw(DirectXBase *dx_base);

	// デプスバッファのクリア
	static void ClearDepthBuffer();
};

