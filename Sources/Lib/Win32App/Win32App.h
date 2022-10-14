#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "../../App/Singleton/Singleton.h"

class Win32App final : public Singleton<Win32App>
{
	friend Singleton<Win32App>;
	using XMINT2 = DirectX::XMINT2;

	static WNDCLASSEX w;	// �E�B���h�E�N���X
	static HWND hwnd;		// �E�B���h�E�I�u�W�F�N�g
	static RECT wrc;		// �E�B���h�E�T�C�Y

public:

	// �E�B���h�E�T�C�Y
	/*static const int win_width_ = 1280;
	static const int win_height_ = 720;*/
	static const XMINT2 SIZE_;

	// �E�B���h�E�T�C�Y�̔���
	/*static const int window_center_x_ = win_width_ / 2;
	static const int window_center_y_ = win_height_ / 2;*/
	static const XMINT2 CENTER_;

public:

	static void StaticInitialize();

	static WNDCLASSEX GetW() { return w; }
	static HWND GetHwnd() { return hwnd; }

	/// <summary>
	/// ���b�Z�[�W�̏���
	/// </summary>
	static bool ProcessMessage();

	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
