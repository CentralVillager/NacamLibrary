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
	// �����X�V
	old_state_ = state_;

	// �[���N���A
	ZeroMemory(&state_, sizeof(XINPUT_STATE));

	// �R���g���[���[�̏�Ԃ��擾
	DWORD result = XInputGetState(user_num_, &state_.state);

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

char NcmInput::GetDirection(NcmStickType type)
{
	using enum NcmStickDirection;

	// �X�e�B�b�N�̏�Ԃ��擾
	StickOutput result = GetStickState(type, state_.state);

	// ���������j���[�g�����ŏ�����
	char direction = (char)(NEUTRAL);

	// ��
	if (IsPlus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(UP);
	}
	// ��
	else if (IsMinus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(DOWN);
	}

	// ��
	if (IsMinus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(LEFT);
	}
	// �E
	else if (IsPlus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(RIGHT);
	}

	return direction;
}

char NcmInputState::GetDirection(NcmStickType type)
{
	using enum NcmStickDirection;

	// �X�e�B�b�N�̏�Ԃ��擾
	StickOutput result = NcmInput::GetStickState(type, state);

	// ���������j���[�g�����ŏ�����
	char direction = (char)(NEUTRAL);

	// ��
	if (IsPlus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(UP);
	}
	// ��
	else if (IsMinus(result.y) && result.x < result.dead_zone && result.x > -result.dead_zone)
	{
		direction |= (char)(DOWN);
	}

	// ��
	if (IsMinus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(LEFT);
	}
	// �E
	else if (IsPlus(result.x) && result.y < result.dead_zone && result.y > -result.dead_zone)
	{
		direction |= (char)(RIGHT);
	}

	return direction;
}
