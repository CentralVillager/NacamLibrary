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
	/// DirectInputデバイス
	/// </summary>
	static ComPtr<IDirectInput8> dinput_;

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// DirectInputのGetter
	/// </summary>
	/// <returns></returns>
	static const ComPtr<IDirectInput8> &GetDirectInput() { return dinput_; }

	/// <summary>
	/// DirectInputデバイスの生成
	/// </summary>
	static void MakeDirectInput();
};
