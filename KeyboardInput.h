#pragma once
#include "InputManager.h"

class KeyboardInput final : public Singleton<KeyboardInput> {
	friend Singleton<KeyboardInput>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	KeyboardInput();

	static ComPtr<IDirectInput8> dinput_;
	static ComPtr<IDirectInputDevice8> dev_keyboard_;

	static BYTE key_[256];
	static BYTE old_key_[256];

	static KeyboardInput *input_;

public:

	static const ComPtr<IDirectInput8> &GetIDirectInput8() { return dinput_; }

	static void Initialize();
	static void Update();
	static void Finalize();

	/// <summary>
	/// �����Ă����true
	/// </summary>
	/// <param name="key">���������擾�������L�[</param>
	/// <returns></returns>
	static bool PushKey(BYTE key);

	/// <summary>
	/// �������u��true
	/// </summary>
	/// <param name="key">���������擾�������L�[</param>
	/// <returns></returns>
	static bool TriggerKey(BYTE key);

	/// <summary>
	/// �����Ă����true
	/// </summary>
	/// <param name="key">���������擾�������L�[</param>
	/// <returns></returns>
	static bool HoldKey(BYTE key);

	/// <summary>
	/// �������u��true
	/// </summary>
	/// <param name="key">���������擾�������L�[</param>
	/// <returns></returns>
	static bool ReleaseKey(BYTE key);

	static void MakeKeyboardDevice();		// �L�[�{�[�h�f�o�C�X�̐���
	static void SetInputDataType();			// ���̓f�[�^�`���̃Z�b�g
	static void SetExclusiveControlLevel();	// �r�����䃌�x���̃Z�b�g

	static void StartGetKeyBoardInfo();		// �L�[�{�[�h���̎擾�J�n
	static void GetAllKeyInput();			// �S�L�[�̓��͏�Ԃ��擾����
};
