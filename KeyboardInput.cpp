#include "KeyboardInput.h"

Microsoft::WRL::ComPtr<IDirectInput8> KeyboardInput::dinput_;
Microsoft::WRL::ComPtr<IDirectInputDevice8> KeyboardInput::dev_keyboard_;
BYTE KeyboardInput::key_[256];
BYTE KeyboardInput::old_key_[256];
KeyboardInput *KeyboardInput::input_ = GetInstance();

KeyboardInput::KeyboardInput() {

	dinput_ = nullptr;
	dev_keyboard_ = nullptr;
}

bool KeyboardInput::PushKey(BYTE key) {

	if (input_->key_[key]) { return true; }

	return false;
}

bool KeyboardInput::TriggerKey(BYTE key) {

	if (input_->key_[key] && !input_->old_key_[key]) { return true; }

	return false;
}

bool KeyboardInput::HoldKey(BYTE key) {

	if (input_->key_[key] && input_->old_key_[key]) { return true; }

	return false;
}

bool KeyboardInput::ReleaseKey(BYTE key) {

	if (!input_->key_[key] && input_->old_key_[key]) { return true; }

	return false;
}

void KeyboardInput::Initialize() {

	dinput_ = InputManager::GetDirectInput();
	MakeKeyboardDevice();
	SetInputDataType();
	SetExclusiveControlLevel();
}

void KeyboardInput::Update() {

	StartGetKeyBoardInfo();
	memcpy(old_key_, key_, sizeof(key_));
	GetAllKeyInput();
}

void KeyboardInput::Finalize() {

	HRESULT result;

	result = dev_keyboard_->Unacquire();
	dev_keyboard_->Release();
	dinput_->Release();
}

// キーボードデバイスの生成
void KeyboardInput::MakeKeyboardDevice() {

	HRESULT result;
	result = dinput_->CreateDevice(GUID_SysKeyboard, &dev_keyboard_, NULL);// GUID_Joystick, GUID_SysMouse...
}

// 入力データ形式のセット
void KeyboardInput::SetInputDataType() {

	HRESULT result;
	result = dev_keyboard_->SetDataFormat(&c_dfDIKeyboard);// 標準形式
}

// 排他制御レベルのセット
void KeyboardInput::SetExclusiveControlLevel() {

	HRESULT result;
	result = dev_keyboard_->SetCooperativeLevel(Window::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/);
}

// キーボード情報の取得開始
void KeyboardInput::StartGetKeyBoardInfo() {

	HRESULT result;
	result = dev_keyboard_->Acquire();// S_OKじゃなくてもスルー
}

// 全キーの入力状態を取得する
void KeyboardInput::GetAllKeyInput() {

	HRESULT result;
	result = dev_keyboard_->GetDeviceState(sizeof(key_), key_);
}