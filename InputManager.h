#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include "Win32App.h"
#include <wrl.h>
#include "Singleton.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class InputManager final : public Singleton<InputManager> {
	friend Singleton<InputManager>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	/// <summary>
	/// DirectInput�f�o�C�X
	/// </summary>
	static ComPtr<IDirectInput8> dinput_;

public:

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// DirectInput��Getter
	/// </summary>
	/// <returns></returns>
	static const ComPtr<IDirectInput8> &GetDirectInput() { return dinput_; }

	/// <summary>
	/// DirectInput�f�o�C�X�̐���
	/// </summary>
	static void MakeDirectInput();
};
