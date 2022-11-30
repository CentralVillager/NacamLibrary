#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>
#include <array>

#pragma comment(lib, "Xinput.lib")

/// <summary>
/// コントローラーのボタンの種類
/// </summary>
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

/// <summary>
/// コントローラー側面のトリガーの種類
/// </summary>
enum class NcmTriggerType
{
	L_TRI, R_TRI
};

/// <summary>
/// コントローラーのスティックの種類
/// </summary>
enum class NcmStickType
{
	L_STICK, R_STICK,
};

/// <summary>
/// スティックの入力方向の種類
/// </summary>
enum class NcmStickDirection : char
{
	NEUTRAL = 0b0000,

	UP = 0b0001,
	DOWN = 0b0010,
	LEFT = 0b0100,
	RIGHT = 0b1000,

	UPPER_LEFT = 0b0101,
	UPPER_RIGHT = 0b1001,
	DOWNER_LEFT = 0b0110,
	DOWNER_RIGHT = 0b1010
};

/// <summary>
/// スティックの入力情報
/// </summary>
struct StickOutput
{
	SHORT x;
	SHORT y;

	int32_t dead_zone;
};

/// <summary>
/// コントローラーの入力情報
/// </summary>
struct NcmInputState
{
	XINPUT_STATE state;
	NcmStickDirection stick_direction;

	/// <summary>
	/// 入力された方向の取得
	/// </summary>
	char GetDirection(NcmStickType type);
};

/// <summary>
/// XInputを使用したコントローラー入力を管理します。
/// </summary>
class NcmInput
{
public:

	// デッドゾーン
	static constexpr uint32_t NCM_LEFT_STICK_DEAD_ZONE_ = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + 500;
	static constexpr uint32_t NCM_RIGHT_STICK_DEAD_ZONE_ = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + 500;

private:

	// コントローラーの認識番号
	static DWORD user_num_;

	// 接続されているか
	static bool is_connected_;

	// 状態を格納する変数
	static NcmInputState state_;

	// 1フレーム前の状態を格納する変数
	static NcmInputState old_state_;

public:

	/// <summary>
	/// 状態の更新
	/// </summary>
	static void Update();

	/// <summary>
	/// デバッグ用描画
	/// </summary>
	static void DebugDraw();

public:

#pragma region Button

	/// <summary>
	/// ボタンが押下されているか
	/// </summary>
	static bool IsPush(NcmButtonType button);

	/// <summary>
	/// ボタンが押された瞬間か
	/// </summary>
	static bool IsTrigger(NcmButtonType button);

	/// <summary>
	/// ボタンが離された瞬間か
	/// </summary>
	static bool IsRelease(NcmButtonType button);

#pragma endregion

#pragma region Trigger

	/// <summary>
	/// トリガーが反応しているか
	/// </summary>
	static bool IsPush(NcmTriggerType trigger);

#pragma endregion

#pragma region Stick

	/// <summary>
	/// スティックが入力されているか
	/// </summary>
	static bool IsHold(NcmStickType type, NcmStickDirection direction);

	/// <summary>
	/// スティックが入力された瞬間か
	/// </summary>
	static bool IsTrigger(NcmStickType type, NcmStickDirection direction);

	/// <summary>
	/// スティックが離された瞬間か
	/// </summary>
	static bool IsRelease(NcmStickType type, NcmStickDirection direction);

	/// <summary>
	/// スティックの入力情報を取得する
	/// </summary>
	static StickOutput GetStickState(NcmStickType stick, XINPUT_STATE state);

#pragma endregion

private:

	/// <summary>
	/// スティックの入力方向を取得する
	/// </summary>
	static char GetDirection(NcmStickType type);
};
