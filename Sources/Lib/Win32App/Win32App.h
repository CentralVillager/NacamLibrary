#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "../../Lib/Singleton/Singleton.h"

class Win32App final : public Singleton<Win32App>
{
	friend Singleton<Win32App>;
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;

	static WNDCLASSEX w;	// �E�B���h�E�N���X
	static HWND hwnd;		// �E�B���h�E�I�u�W�F�N�g
	static RECT wrc;		// �E�B���h�E�T�C�Y

public:

	// �E�B���h�E�T�C�Y
	static constexpr XMINT2 SIZE_ = { 1280, 720 };
	static constexpr XMFLOAT2 FSIZE_ = { 1280.0f, 720.0f };

	// �E�B���h�E�T�C�Y�̔���
	static constexpr XMINT2 CENTER_ = { SIZE_.x / 2, SIZE_.y / 2 };
	static constexpr XMFLOAT2 FCENTER_ = { FSIZE_.x / 2.0f, FSIZE_.y / 2.0f };

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
