#include "../Input/ControllerInput.h"
#include "../Input/InputManager.h"
#include "../../App/Debug/NcmImGui.h"

Microsoft::WRL::ComPtr<IDirectInput8> ControllerInput::dinput_;
Microsoft::WRL::ComPtr<IDirectInputDevice8> ControllerInput::dev_controller_;
DIJOYSTATE ControllerInput::pad_data_;
DIJOYSTATE ControllerInput::old_pad_data_;

void ControllerInput::Initialize() {

	ControllerInput::dinput_ = InputManager::GetDirectInput();

	// コントローラデバイスが生成されなかったら処理を抜ける
	if (!MakeControllerDevice()) {

		return;
	}

	SetFormat();
	SetProperty();
	SetExclusiveControlLevel();
}

void ControllerInput::Update() {

	HRESULT result;

	// コントローラーが接続されていなかったらスルー
	if (!dev_controller_) { return; }

	// デバイス情報の取得開始
	result = dev_controller_->Acquire();// S_OKじゃなくてもスルー

	// デバイス情報の同期
	result = dev_controller_->Poll();

	memcpy(&old_pad_data_, &pad_data_, sizeof(pad_data_));

	// 入力情報の取得
	result = dev_controller_->GetDeviceState(sizeof(DIJOYSTATE), &pad_data_);
}

void ControllerInput::Finalize() {

	HRESULT result;

	if (!dev_controller_) { return; }

	result = dev_controller_->Unacquire();
	dev_controller_->Release();
	dinput_->Release();
}

bool ControllerInput::LStickLeft() {
	if (pad_data_.lX < -DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::LStickRight() {
	if (pad_data_.lX > DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::LStickUp() {
	if (pad_data_.lY < -DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::LStickDown() {
	if (pad_data_.lY > DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::RStickLeft() {
	if (pad_data_.lRx < -DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::RStickRight() {
	if (pad_data_.lRx > DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::RStickUp() {
	if (pad_data_.lRy < -DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::RStickDown() {
	if (pad_data_.lRy > DEAD_ZONE_) { return true; }
	return false;
}

bool ControllerInput::PushCrossKey(BYTE angle) {
	if (pad_data_.rgdwPOV[0] != 0xffffffff) {
		if (pad_data_.rgdwPOV[0] == angle * 100) {
			return true;
		}
	}

	return false;
}

bool ControllerInput::TriggerCrossKey(BYTE angle) {
	return false;
}

bool ControllerInput::ReleaseCrossKey(BYTE angle) {
	return false;
}

bool ControllerInput::PushButton(BYTE button) {
	if (pad_data_.rgbButtons[button] & 0x80) { return true; }
	return false;
}

bool ControllerInput::TriggerButton(BYTE button) {
	if (!(old_pad_data_.rgbButtons[button] & 0x80)
		&& pad_data_.rgbButtons[button] & 0x80) {
		return true;
	}

	return false;
}

bool ControllerInput::ReleaseButton(BYTE button) {
	if (old_pad_data_.rgbButtons[button] & 0x80
		&& !(pad_data_.rgbButtons[button] & 0x80)) {
		return true;
	}

	return false;
}

bool ControllerInput::MakeControllerDevice() {
	HRESULT result;
	result = dinput_->CreateDevice(GUID_Joystick, &dev_controller_, NULL);// GUID_Joystick, GUID_SysMouse...

	if (dev_controller_) {

		return true;

	} else {

		return false;
	}
}

void ControllerInput::SetFormat() {
	HRESULT result;
	result = dev_controller_->SetDataFormat(&c_dfDIJoystick);
}

void ControllerInput::SetProperty() {
	HRESULT result;

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD di_prop = {};
	di_prop.diph.dwSize = sizeof(di_prop);
	di_prop.diph.dwHeaderSize = sizeof(di_prop.diph);
	di_prop.diph.dwHow = DIPH_DEVICE;
	di_prop.diph.dwObj = 0;
	di_prop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定

	result = dev_controller_->SetProperty(DIPROP_AXISMODE, &di_prop.diph);

	// X軸の値の範囲設定
	DIPROPRANGE di_range = {};
	di_range.diph.dwSize = sizeof(di_range);
	di_range.diph.dwHeaderSize = sizeof(di_range.diph);
	di_range.diph.dwHow = DIPH_BYOFFSET;
	di_range.diph.dwObj = DIJOFS_X;
	di_range.lMin = -1000;
	di_range.lMax = 1000;

	result = dev_controller_->SetProperty(DIPROP_RANGE, &di_range.diph);

	// Y軸の値の範囲設定
	di_range.diph.dwObj = DIJOFS_Y;
	result = dev_controller_->SetProperty(DIPROP_RANGE, &di_range.diph);
}

void ControllerInput::SetExclusiveControlLevel() {
	HRESULT result;

	result = dev_controller_->SetCooperativeLevel(Win32App::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void ControllerInput::DebugDraw() {
	ImGui::Text("Controller");
	ImGui::Text("A : %d", PushButton(DIB_A));
	ImGui::Text("B : %d", PushButton(DIB_B));
	ImGui::Text("X : %d", PushButton(DIB_X));
	ImGui::Text("Y : %d", PushButton(DIB_Y));
	ImGui::Text("L : %d", PushButton(DIB_L));
	ImGui::Text("R : %d", PushButton(DIB_R));
	ImGui::Text("UP : %d", PushCrossKey(DIB_UP));
	ImGui::Text("RIGHT : %d", PushCrossKey(DIB_RIGHT));
	ImGui::Text("DOWN : %d", PushCrossKey(DIB_DOWN));
	ImGui::Text("LEFT : %d", PushCrossKey(DIB_LEFT));
}