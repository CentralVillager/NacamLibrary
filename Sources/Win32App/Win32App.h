#pragma once
#include <Windows.h>
#include "../Singleton/Singleton.h"

class Win32App final : public Singleton<Win32App> {
	friend Singleton<Win32App>;

	// �E�B���h�E�N���X
	static WNDCLASSEX w;
	// �E�B���h�E�I�u�W�F�N�g
	static HWND hwnd;
	// �E�B���h�E�T�C�Y
	static RECT wrc;

public:
	// �E�B���h�E�T�C�Y
	static const int window_width_ = 1280;
	static const int window_height_ = 720;

	// �E�B���h�E�T�C�Y�̔���
	static const int window_center_x_ = window_width_ / 2;
	static const int window_center_y_ = window_height_ / 2;

public:
	static void StaticInitialize();

	static WNDCLASSEX GetW() { return w; }
	static HWND GetHwnd() { return hwnd; }
	static int GetWindowWidth() { return window_width_; }
	static int GetWindowHeight() { return window_height_; }

	/// <summary>
	/// ���b�Z�[�W�̏���
	/// </summary>
	static bool ProcessMessage();

	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
