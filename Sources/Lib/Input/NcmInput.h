#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>

#pragma comment(lib, "Xinput.lib")

enum class NcmButtonType
{
	// �\���L�[
	UP = XINPUT_GAMEPAD_DPAD_UP,
	DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,

	// �X�^�[�g�{�^����
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,

	// �X�e�B�b�N��������
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
	// �R���g���[���[�̔F���ԍ�
	DWORD user_num_;

	// �ڑ�����Ă��邩
	bool is_connected_;

	// ��Ԃ��i�[����ϐ�
	XINPUT_STATE state_;

	// 1�t���[���O�̏�Ԃ��i�[����ϐ�
	XINPUT_STATE old_state_;

public:

	NcmInput();
	~NcmInput();

public:

	void Update();
	void DebugDraw();

public:

	/// <summary>
	/// �{�^������������Ă��邩
	/// </summary>
	bool IsPush(NcmButtonType button);

	/// <summary>
	/// �{�^���������ꂽ�u�Ԃ�
	/// </summary>
	bool IsTrigger(NcmButtonType button);

	/// <summary>
	/// �{�^���������ꂽ�u�Ԃ�
	/// </summary>
	bool IsRelease(NcmButtonType button);

	/// <summary>
	/// �g���K�[���������Ă��邩
	/// </summary>
	bool IsPush(NcmTriggerType trigger);

	/// <summary>
	/// �X�e�B�b�N�����擾����
	/// </summary>
	StickInput GetStickState(NcmStickType stick);

	/// <summary>
	/// �\�������Ɋۂ߂�ꂽ�X�e�B�b�N�����擾����
	/// </summary>
	StickInput GetStickStateLikeCross(NcmStickType stick);
};
