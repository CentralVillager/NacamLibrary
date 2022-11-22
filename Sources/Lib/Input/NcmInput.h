#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>

#pragma comment(lib, "Xinput.lib")

enum class NcmButtonType
{
	// 十字キー
	UP = XINPUT_GAMEPAD_DPAD_UP,
	DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,

	// スタートボタン他
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,

	// スティック押し込み
	LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB,
	RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB,

	// LB, RB
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,

	// A, B, X, Y
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
};

enum class NcmTriggerType
{
	L_TRI,
	R_TRI
};

enum class NcmStickType
{
	L_STICK,
	R_STICK,
};

struct StickInput
{
	SHORT x;
	SHORT y;

	int32_t dead_zone;
};

class NcmInput
{
	// コントローラーの認識番号
	DWORD user_num_;

	// 接続されているか
	bool is_connected_;

	// 状態を格納する変数
	XINPUT_STATE state_;

	// 1フレーム前の状態を格納する変数
	XINPUT_STATE old_state_;

public:

	NcmInput();
	~NcmInput();

public:

	void Update();
	void DebugDraw();

public:

	/// <summary>
	/// ボタンが押下されているか
	/// </summary>
	bool IsPush(NcmButtonType button);

	/// <summary>
	/// ボタンが押された瞬間か
	/// </summary>
	bool IsTrigger(NcmButtonType button);

	/// <summary>
	/// ボタンが離された瞬間か
	/// </summary>
	bool IsRelease(NcmButtonType button);

	/// <summary>
	/// トリガーが反応しているか
	/// </summary>
	bool IsPush(NcmTriggerType trigger);

	/// <summary>
	/// スティック情報を取得する
	/// </summary>
	StickInput GetStickState(NcmStickType stick);

	/// <summary>
	/// 十字方向に丸められたスティック情報を取得する
	/// </summary>
	StickInput GetStickStateLikeCross(NcmStickType stick);
};
