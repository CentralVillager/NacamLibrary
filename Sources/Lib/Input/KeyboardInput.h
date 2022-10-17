#pragma once
#include "../Input/InputManager.h"

class KeyboardInput final : public Singleton<KeyboardInput>
{
	friend Singleton<KeyboardInput>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	KeyboardInput();

	static ComPtr<IDirectInput8> dinput_;
	static ComPtr<IDirectInputDevice8> dev_keyboard_;

	static constexpr int MAX_KEY_NUM_ = 256;

	static BYTE key_[MAX_KEY_NUM_];
	static BYTE old_key_[MAX_KEY_NUM_];

	static KeyboardInput *input_;

public:

	static const ComPtr<IDirectInput8> &GetIDirectInput8() { return dinput_; }

	static void Initialize();
	static void Update();
	static void Finalize();

	/// <summary>
	/// 押している間true
	/// </summary>
	/// <param name="key">押下情報を取得したいキー</param>
	/// <returns></returns>
	static bool PushKey(BYTE key);

	/// <summary>
	/// 押した瞬間true
	/// </summary>
	/// <param name="key">押下情報を取得したいキー</param>
	/// <returns></returns>
	static bool TriggerKey(BYTE key);

	/// <summary>
	/// 押している間true
	/// </summary>
	/// <param name="key">押下情報を取得したいキー</param>
	/// <returns></returns>
	static bool HoldKey(BYTE key);

	/// <summary>
	/// 離した瞬間true
	/// </summary>
	/// <param name="key">押下情報を取得したいキー</param>
	/// <returns></returns>
	static bool ReleaseKey(BYTE key);

	static void MakeKeyboardDevice();		// キーボードデバイスの生成
	static void SetInputDataType();			// 入力データ形式のセット
	static void SetExclusiveControlLevel();	// 排他制御レベルのセット

	static void StartGetKeyBoardInfo();		// キーボード情報の取得開始
	static void GetAllKeyInput();			// 全キーの入力状態を取得する
};
