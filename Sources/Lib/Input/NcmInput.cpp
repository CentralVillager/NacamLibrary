#include "NcmInput.h"
#include "../../App/Debug/NcmImGui.h"
#include "../../App/Utility/NcmUtil.h"

using namespace NcmUtill;

DWORD NcmInput::user_num_ = 0;
bool NcmInput::is_connected_ = false;
NcmInputState NcmInput::state_{};
NcmInputState NcmInput::old_state_{};

void NcmInput::Update()
{
	// 情報を更新
	old_state_ = state_;

	// ゼロクリア
	ZeroMemory(&state_, sizeof(XINPUT_STATE));

	// コントローラーの状態を取得
	DWORD result = XInputGetState(user_num_, &state_.state);

	if (result == ERROR_SUCCESS)
	{
		// 接続されている
		is_connected_ = true;
	}
	else
	{
		// 接続されていない
		is_connected_ = false;
	}
}

void NcmInput::DebugDraw()
{}

bool NcmInput::IsPush(NcmButtonType button)
{
	if (state_.state.Gamepad.wButtons & (int)(button))
	{
		return true;
	}

	return false;
}

bool NcmInput::IsTrigger(NcmButtonType button)
{
	if (state_.state.Gamepad.wButtons & (int)(button) && !(old_state_.state.Gamepad.wButtons & (int)(button)))
	{
		return true;
	}

	return false;
}

bool NcmInput::IsRelease(NcmButtonType button)
{
	if (!(state_.state.Gamepad.wButtons & (int)(button)) && old_state_.state.Gamepad.wButtons & (int)(button))
	{
		return true;
	}

	return false;
}

bool NcmInput::IsPush(NcmTriggerType trigger)
{
	BYTE trigger_type;
	ZeroMemory(&trigger_type, sizeof(BYTE));

	if (trigger == NcmTriggerType::L_TRI)
	{
		trigger_type = state_.state.Gamepad.bLeftTrigger;
	}
	else if (trigger == NcmTriggerType::R_TRI)
	{
		trigger_type = state_.state.Gamepad.bRightTrigger;
	}

	if (trigger_type > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return true;
	}

	return false;
}

bool NcmInput::IsHold(NcmStickType type, NcmStickDirection direction)
{
	using enum NcmStickDirection;

	if (GetDirection(type) & (char)(direction))
	{
		return true;
	}

	return false;
}

bool NcmInput::IsTrigger(NcmStickType type, NcmStickDirection direction)
{
	using enum NcmStickDirection;

	if (state_.GetDirection(type) & (char)(direction) &&
		!(old_state_.GetDirection(type) & (char)(direction)))
	{
		return true;
	}

	return false;
}

bool NcmInput::IsRelease(NcmStickType type, NcmStickDirection direction)
{
	using enum NcmStickDirection;

	if (!(state_.GetDirection(type) & (char)(direction)) &&
		(old_state_.GetDirection(type) & (char)(direction)))
	{
		return true;
	}

	return false;
}

StickOutput NcmInput::GetStickState(NcmStickType stick, XINPUT_STATE state)
{
	StickOutput input{};
	ZeroMemory(&input, sizeof(StickOutput));

	if (stick == NcmStickType::L_STICK)
	{
		input.x = state.Gamepad.sThumbLX;
		input.y = state.Gamepad.sThumbLY;
		input.dead_zone = NCM_LEFT_STICK_DEAD_ZONE_;
	}
	else if (stick == NcmStickType::R_STICK)
	{
		input.x = state.Gamepad.sThumbRX;
		input.y = state.Gamepad.sThumbRY;
		input.dead_zone = NCM_RIGHT_STICK_DEAD_ZONE_;
	}

	// デッドゾーン以内の入力を丸める
	if ((input.x <  input.dead_zone &&
		input.x > -input.dead_zone) &&
		(input.y <  input.dead_zone &&
			input.y > -input.dead_zone))
	{
		input.x = 0;
		input.y = 0;
	}

	return input;
}

char NcmInput::GetDirection(NcmStickType type)
{
	using enum NcmStickDirection;

	// スティックの状態を取得
	StickOutput result = GetStickState(type, state_.state);

	// 方向情報をニュートラルで初期化
	char direction = (char)(NEUTRAL);

	// 上
	if (IsPlus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(UP);
	}
	// 下
	else if (IsMinus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(DOWN);
	}

	// 左
	if (IsMinus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(LEFT);
	}
	// 右
	else if (IsPlus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(RIGHT);
	}

	return direction;
}

char NcmInputState::GetDirection(NcmStickType type)
{
	using enum NcmStickDirection;

	// スティックの状態を取得
	StickOutput result = NcmInput::GetStickState(type, state);

	// 方向情報をニュートラルで初期化
	char direction = (char)(NEUTRAL);

	// 上
	if (IsPlus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(UP);
	}
	// 下
	else if (IsMinus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(DOWN);
	}

	// 左
	if (IsMinus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(LEFT);
	}
	// 右
	else if (IsPlus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(RIGHT);
	}

	return direction;
}
