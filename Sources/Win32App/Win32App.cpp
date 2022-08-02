#include "../Win32App/Win32App.h"

WNDCLASSEX Win32App::w{};
HWND Win32App::hwnd;
RECT Win32App::wrc;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

void Win32App::StaticInitialize() {
	// ウィンドウクラスの設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = static_cast<WNDPROC>(WindowProc);	// ウィンドウプロシージャを指定
	w.lpszClassName = L"NacamLibrary";					// ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);				// ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);			// カーソル指定

	RegisterClassEx(&w);								// ウィンドウクラスをOSに指定

	// ウィンドウサイズ
	wrc = { 0, 0, window_width_, window_height_ };

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(
		w.lpszClassName,		// クラス名
		L"NacamLibrary",		// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// 標準的なウィンドウスタイル
		CW_USEDEFAULT,			// 表示X座標（OSに任せる）
		CW_USEDEFAULT,			// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		w.hInstance,			// 呼び出しアプリケーションハンドル
		nullptr					// オプション
	);

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

bool Win32App::ProcessMessage() {
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	// キー入力メッセージの処理
		DispatchMessage(&msg);	// プロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) {
		return false;
	}

	return true;
}

LRESULT Win32App::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
