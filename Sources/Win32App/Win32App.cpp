#include "../Win32App/Win32App.h"

WNDCLASSEX Win32App::w{};
HWND Win32App::hwnd;
RECT Win32App::wrc;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

void Win32App::StaticInitialize() {
	// �E�B���h�E�N���X�̐ݒ�
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = static_cast<WNDPROC>(WindowProc);	// �E�B���h�E�v���V�[�W�����w��
	w.lpszClassName = L"NacamLibrary";					// �E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);				// �E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);			// �J�[�\���w��

	RegisterClassEx(&w);								// �E�B���h�E�N���X��OS�Ɏw��

	// �E�B���h�E�T�C�Y
	wrc = { 0, 0, window_width_, window_height_ };

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(
		w.lpszClassName,		// �N���X��
		L"NacamLibrary",		// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,			// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,			// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,	// �E�B���h�E����
		wrc.bottom - wrc.top,	// �E�B���h�E�c��
		nullptr,				// �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		w.hInstance,			// �Ăяo���A�v���P�[�V�����n���h��
		nullptr					// �I�v�V����
	);

	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

bool Win32App::ProcessMessage() {
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	// �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	// �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
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
