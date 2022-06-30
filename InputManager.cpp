#include "InputManager.h"

Microsoft::WRL::ComPtr<IDirectInput8> InputManager::dinput_;

void InputManager::StaticInitialize() {

	MakeDirectInput();
}

void InputManager::MakeDirectInput() {

	HRESULT result;

	result = DirectInput8Create(
		Win32App::GetW().hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void **)&dinput_,
		nullptr);
}
