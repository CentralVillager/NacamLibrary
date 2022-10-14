#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "../../App/Singleton/Singleton.h"

class Win32App final : public Singleton<Win32App>
{
	friend Singleton<Win32App>;
	using XMINT2 = DirectX::XMINT2;

	static WNDCLASSEX w;	// ウィンドウクラス
	static HWND hwnd;		// ウィンドウオブジェクト
	static RECT wrc;		// ウィンドウサイズ

public:

	// ウィンドウサイズ
	/*static const int win_width_ = 1280;
	static const int win_height_ = 720;*/
	static const XMINT2 SIZE_;

	// ウィンドウサイズの半分
	/*static const int window_center_x_ = win_width_ / 2;
	static const int window_center_y_ = win_height_ / 2;*/
	static const XMINT2 CENTER_;

public:

	static void StaticInitialize();

	static WNDCLASSEX GetW() { return w; }
	static HWND GetHwnd() { return hwnd; }

	/// <summary>
	/// メッセージの処理
	/// </summary>
	static bool ProcessMessage();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
