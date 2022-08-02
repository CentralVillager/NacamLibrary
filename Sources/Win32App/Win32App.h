#pragma once
#include <Windows.h>
#include "../Singleton/Singleton.h"

class Win32App final : public Singleton<Win32App> {
	friend Singleton<Win32App>;

	// ウィンドウクラス
	static WNDCLASSEX w;
	// ウィンドウオブジェクト
	static HWND hwnd;
	// ウィンドウサイズ
	static RECT wrc;

public:
	// ウィンドウサイズ
	static const int window_width_ = 1280;
	static const int window_height_ = 720;

	// ウィンドウサイズの半分
	static const int window_center_x_ = window_width_ / 2;
	static const int window_center_y_ = window_height_ / 2;

public:
	static void StaticInitialize();

	static WNDCLASSEX GetW() { return w; }
	static HWND GetHwnd() { return hwnd; }
	static int GetWindowWidth() { return window_width_; }
	static int GetWindowHeight() { return window_height_; }

	/// <summary>
	/// メッセージの処理
	/// </summary>
	static bool ProcessMessage();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
