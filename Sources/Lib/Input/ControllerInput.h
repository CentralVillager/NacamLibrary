#pragma once
#include "../Input/InputManager.h"

// Direct Input Button
#define DIB_A		0
#define DIB_B		1
#define DIB_X		2
#define DIB_Y		3
#define DIB_L		4
#define DIB_R		5
#define DIB_SUB_1	6
#define DIB_SUB_2	7
#define DIB_L3		8
#define DIB_R3		9

#define DIB_UP		0
#define DIB_RIGHT	9
#define DIB_DOWN	18
#define DIB_LEFT	27

class ControllerInput final : public Singleton<ControllerInput>
{
	friend Singleton<ControllerInput>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	static ComPtr<IDirectInput8> dinput_;
	static ComPtr<IDirectInputDevice8> dev_controller_;

	// �f�o�C�X���i�[�\����
	static DIJOYSTATE pad_data_;
	static DIJOYSTATE old_pad_data_;

	// �f�b�h�]�[��
	static const long DEAD_ZONE_ = 200;

	// �ő��t�\��
	static const short MAX_BUTTON_NUM_ = 32;

public:

	/// <summary>
	/// ���E�X�e�B�b�N�̔��ʗp
	/// </summary>
	enum class StickType
	{
		RIGHT,
		LEFT
	};

	/// <summary>
	/// �A�i���O�X�e�B�b�N�̌����Ă������
	/// </summary>
	enum class StickWay
	{
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

	static void Initialize();
	static void Finalize();
	static void Update();
	static void DebugDraw();

	static bool LStickLeft();
	static bool LStickRight();
	static bool LStickUp();
	static bool LStickDown();

	static bool RStickLeft();
	static bool RStickRight();
	static bool RStickUp();
	static bool RStickDown();

	static bool PushCrossKey(BYTE angle);
	static bool TriggerCrossKey(BYTE angle);
	static bool ReleaseCrossKey(BYTE angle);

	static bool PushButton(BYTE button);
	static bool TriggerButton(BYTE button);
	static bool ReleaseButton(BYTE button);

	/// <summary>
	/// �R���g���[���[�f�o�C�X�̐���
	/// </summary>
	static bool MakeControllerDevice();

	/// <summary>
	/// �f�o�C�X�̃t�H�[�}�b�g��ݒ�
	/// </summary>
	static void SetFormat();

	/// <summary>
	/// �v���p�e�B�̐ݒ�
	/// </summary>
	static void SetProperty();

	/// <summary>
	/// �r�����䃌�x���̃Z�b�g
	/// </summary>
	static void SetExclusiveControlLevel();
};
