#include "NcmInput.h"
#include "../../App/Debug/NcmImGui.h"
#include "../../App/Utility/NcmUtil.h"

using namespace NcmUtill;

NcmInput::NcmInput() :
	user_num_(),
	is_connected_(false),
	state_(),
	old_state_()
{}

NcmInput::~NcmInput()
{}

void NcmInput::Update()
{
	// �����X�V
	old_state_ = state_;

	// �[���N���A
	ZeroMemory(&state_, sizeof(XINPUT_STATE));

	// �R���g���[���[�̏�Ԃ��擾
	DWORD result = XInputGetState(user_num_, &state_);

	if (result == ERROR_SUCCESS)
	{
		// �ڑ�����Ă���
		is_connected_ = true;
	}
	else
	{
		// �ڑ�����Ă��Ȃ�
		is_connected_ = false;
	}
}

void NcmInput::DebugDraw()
{
	using enum NcmStickType;

	ImGui::Text("%d, %d", GetStickStateLikeCross(L_STICK).x, GetStickStateLikeCross(L_STICK).y);
	ImGui::Text("%d, %d", GetStickStateLikeCross(R_STICK).x, GetStickStateLikeCross(R_STICK).y);
}

bool NcmInput::IsPush(NcmButtonType button)
{
	if (state_.Gamepad.wButtons & (int)(button))
	{
		return true;
	}

	return false;
}

bool NcmInput::IsTrigger(NcmButtonType button)
{
	if (state_.Gamepad.wButtons & (int)(button) && !(old_state_.Gamepad.wButtons & (int)(button)))
	{
		return true;
	}

	return false;
}

bool NcmInput::IsRelease(NcmButtonType button)
{
	if (!(state_.Gamepad.wButtons & (int)(button)) && old_state_.Gamepad.wButtons & (int)(button))
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
		trigger_type = state_.Gamepad.bLeftTrigger;
	}
	else if (trigger == NcmTriggerType::R_TRI)
	{
		trigger_type = state_.Gamepad.bRightTrigger;
	}

	if (trigger_type > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return true;
	}

	return false;
}

StickInput NcmInput::GetStickState(NcmStickType stick)
{
	StickInput input{};
	ZeroMemory(&input, sizeof(StickInput));

	if (stick == NcmStickType::L_STICK)
	{
		input.x = state_.Gamepad.sThumbLX;
		input.y = state_.Gamepad.sThumbLY;
		input.dead_zone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + 500;
	}
	else if (stick == NcmStickType::R_STICK)
	{
		input.x = state_.Gamepad.sThumbRX;
		input.y = state_.Gamepad.sThumbRY;
		input.dead_zone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + 500;
	}

	// �f�b�h�]�[���ȓ��̓��͂��ۂ߂�
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

StickInput NcmInput::GetStickStateLikeCross(NcmStickType stick)
{
	StickInput result = GetStickState(stick);

	// ��
	if (IsPlus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		result.y = 1;
	}
	// ��
	else if (IsMinus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		result.y = -1;
	}
	else
	{
		result.y = 0;
	}

	// ��
	if (IsMinus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		result.x = -1;
	}
	// �E
	else if (IsPlus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		result.x = 1;
	}
	else
	{
		result.x = 0;
	}

	return result;
}
