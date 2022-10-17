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
	static const XMINT2 SIZE_;

	// ウィンドウサイズの半分
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
