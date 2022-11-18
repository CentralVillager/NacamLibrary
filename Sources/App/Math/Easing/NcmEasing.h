#pragma once
#include <list>
#include <array>

typedef uint32_t ncm_ehandle;

enum class NcmEaseType
{
	Lerp,

	InQuad,
	InCubic,
	InQuart,
	InQuint,
	InSine,
	InCirc,
	InExpo,
	InBack,
	InElastic,
	InBounce,

	OutQuad,
	OutCubic,
	OutQuart,
	OutQuint,
	OutSine,
	OutCirc,
	OutExpo,
	OutBack,
	OutElastic,
	OutBounce,

	MaxEaseNum
};

struct NcmEaseDesc
{
	// �����l
	float init_value = 0.0f;
	// ���ړ���
	float total_move = 0.0f;
	// �C�[�W���O�̎d��
	NcmEaseType ease_type;
	float t_rate;
	float t_max = 1.0f;

	// ���ʗp�n���h��
	ncm_ehandle handle;
	// �ω���
	float t;
	// �C�[�W���O�֐��̖߂�l�̊i�[��
	float ease_value = 0.0f;

	NcmEaseDesc() :
		init_value(),
		total_move(),
		ease_type(),
		t_rate(),
		t_max(1.0f),
		handle(),
		t(),
		ease_value()
	{}
};

class NcmEasing
{
	// �C�[�W���O�ɕK�v�ȃf�[�^
	static std::list<NcmEaseDesc> easing_datas_;

	// �֐��|�C���^
	typedef float (*EaseFunc)(const float v);

	// �C�[�W���O�֐��̊i�[��
	static std::array<EaseFunc, (size_t)(NcmEaseType::MaxEaseNum)> ease_types_;

	// ���n���h���J�E���g�ϐ�
	static ncm_ehandle handle_counter_;

public:

	NcmEasing();
	~NcmEasing();
	static void StaticInit();

public:
	
	/// <summary>
	/// �C�[�W���O�̐ݒ��o�^���܂��B
	/// </summary>
	/// <returns>�n���h��</returns>
	static ncm_ehandle RegisterEaseData(const NcmEaseDesc &args);

	/// <summary>
	/// �C�[�W���O�̐ݒ���폜���܂��B
	/// </summary>
	/// <param name="handle">�폜�������ݒ�̃n���h��</param>
	static void DeleteEaseData(ncm_ehandle handle);

	/// <summary>
	/// �l���X�V���܂��B
	/// </summary>
	static void UpdateValue(int handle);

	/// <summary>
	/// �l�����Z�b�g���܂��B
	/// </summary>
	static void ResetTime(int handle);

	/// <summary>
	/// ���݂̒l���擾���܂��B
	/// </summary>
	/// <returns>���݂̒l</returns>
	static float GetValue(int handle);

	/// <summary>
	/// �V���������l��ݒ肵�܂��B
	/// </summary>
	/// <param name="handle">�ړI�̃n���h��</param>
	/// <param name="v">�ݒ肵�����l</param>
	static void SetInitValue(int handle, float v);

	/// <summary>
	/// �V�������ړ��ʂ�ݒ肵�܂��B
	/// </summary>
	/// <param name="handle">�ړI�̃n���h��</param>
	/// <param name="t">�ݒ肵�����l</param>
	static void SetTotalMove(int handle, float v);

private:

	// �n���h�������Ɋi�[����Ă���f�[�^���擾
	static NcmEaseDesc *SearchValue(ncm_ehandle handle);

	// t���X�V������
	static void ConvertRate(float *t, float rate = 0.1f, float max = 1.0f);

private:

	static float Lerp(const float t);

	static float InSine(const float t);
	static float InCubic(const float t);
	static float InQuint(const float t);
	static float InCirc(const float t);
	static float InElastic(const float t);
	static float InQuad(const float t);
	static float InQuart(const float t);
	static float InExpo(const float t);
	static float InBack(const float t);
	static float InBounce(const float t);

	static float OutSine(const float t);
	static float OutCubic(const float t);
	static float OutQuint(const float t);
	static float OutCirc(const float t);
	static float OutElastic(const float t);
	static float OutQuad(const float t);
	static float OutQuart(const float t);
	static float OutExpo(const float t);
	static float OutBack(const float t);
	static float OutBounce(float t);

	static float InOutSine(const float t);
	static float InOutCubic(const float t);
	static float InOutQuint(const float t);
	static float InOutCirc(const float t);
	static float InOutElastic(const float t);
	static float InOutQuad(const float t);
	static float InOutQuart(const float t);
	static float InOutExpo(const float t);
	static float InOutBack(const float t);
};
