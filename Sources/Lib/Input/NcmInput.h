#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>
#include <array>

#pragma comment(lib, "Xinput.lib")

/// <summary>
/// �R���g���[���[�̃{�^���̎��
/// </summary>
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

/// <summary>
/// �R���g���[���[���ʂ̃g���K�[�̎��
/// </summary>
enum class NcmTriggerType
{
	L_TRI, R_TRI
};

/// <summary>
/// �R���g���[���[�̃X�e�B�b�N�̎��
/// </summary>
enum class NcmStickType
{
	L_STICK, R_STICK,
};

/// <summary>
/// �X�e�B�b�N�̓��͕����̎��
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
/// �X�e�B�b�N�̓��͏��
/// </summary>
struct StickOutput
{
	SHORT x;
	SHORT y;

	int32_t dead_zone;
};

/// <summary>
/// �R���g���[���[�̓��͏��
/// </summary>
struct NcmInputState
{
	XINPUT_STATE state;
	NcmStickDirection stick_direction;

	/// <summary>
	/// ���͂��ꂽ�����̎擾
	/// </summary>
	char GetDirection(NcmStickType type);
};

/// <summary>
/// XInput���g�p�����R���g���[���[���͂��Ǘ����܂��B
/// </summary>
class NcmInput
{
public:

	// �f�b�h�]�[��
	static constexpr uint32_t NCM_LEFT_STICK_DEAD_ZONE_ = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + 500;
	static constexpr uint32_t NCM_RIGHT_STICK_DEAD_ZONE_ = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + 500;

private:

	// �R���g���[���[�̔F���ԍ�
	static DWORD user_num_;

	// �ڑ�����Ă��邩
	static bool is_connected_;

	// ��Ԃ��i�[����ϐ�
	static NcmInputState state_;

	// 1�t���[���O�̏�Ԃ��i�[����ϐ�
	static NcmInputState old_state_;

public:

	/// <summary>
	/// ��Ԃ̍X�V
	/// </summary>
	static void Update();

	/// <summary>
	/// �f�o�b�O�p�`��
	/// </summary>
	static void DebugDraw();

public:

#pragma region Button

	/// <summary>
	/// �{�^������������Ă��邩
	/// </summary>
	static bool IsPush(NcmButtonType button);

	/// <summary>
	/// �{�^���������ꂽ�u�Ԃ�
	/// </summary>
	static bool IsTrigger(NcmButtonType button);

	/// <summary>
	/// �{�^���������ꂽ�u�Ԃ�
	/// </summary>
	static bool IsRelease(NcmButtonType button);

#pragma endregion

#pragma region Trigger

	/// <summary>
	/// �g���K�[���������Ă��邩
	/// </summary>
	static bool IsPush(NcmTriggerType trigger);

#pragma endregion

#pragma region Stick

	/// <summary>
	/// �X�e�B�b�N�����͂���Ă��邩
	/// </summary>
	static bool IsHold(NcmStickType type, NcmStickDirection direction);

	/// <summary>
	/// �X�e�B�b�N�����͂��ꂽ�u�Ԃ�
	/// </summary>
	static bool IsTrigger(NcmStickType type, NcmStickDirection direction);

	/// <summary>
	/// �X�e�B�b�N�������ꂽ�u�Ԃ�
	/// </summary>
	static bool IsRelease(NcmStickType type, NcmStickDirection direction);

	/// <summary>
	/// �X�e�B�b�N�̓��͏����擾����
	/// </summary>
	static StickOutput GetStickState(NcmStickType stick, XINPUT_STATE state);

#pragma endregion

private:

	/// <summary>
	/// �X�e�B�b�N�̓��͕������擾����
	/// </summary>
	static char GetDirection(NcmStickType type);
};
