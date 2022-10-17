#pragma once
#include <list>
#include <array>

enum class EaseType
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

struct EaseArgs
{
	// ���ʗp�n���h��
	int handle;
	// �ω���
	float t;

	// �����l
	float init_value = 0.0f;
	// ���ړ���
	float total_move = 0.0f;
	// �C�[�W���O�̎d��
	EaseType ease_type;
	// �C�[�W���O�֐��̖߂�l�̊i�[��
	float ease_value = 0.0f;
};

class NcmEasing
{
	// �C�[�W���O�ɕK�v�ȃf�[�^
	static std::list<EaseArgs> easing_datas_;

	// �֐��|�C���^
	typedef float (*EaseFunc)(const float v);

	// �C�[�W���O�֐��̊i�[��
	static std::array<EaseFunc, (size_t)(EaseType::MaxEaseNum)> ease_types_;

	static constexpr float PI_ = 3.14159265358979f;
	static int handle_counter_;

public:

	NcmEasing();
	~NcmEasing();
	static void StaticInit();

public:

	static int RegisterEaseData(const EaseArgs &args);

	static void UpdateValue(int handle);
	static void ResetTime();
	static void ResetTime(int handle);

	static float GetValue(int handle);

	static void SetInitValue(int handle, float v);
	static void SetTotalMove(int handle, float v);

private:

	static EaseArgs *SearchValue(int handle);

	static float MakeEase(float *param, const EaseArgs &ease_args);
	static inline void ConvertRate(float *t, float rate = 0.1f, float max = 1.0f)
	{
		if (*t >= max)
		{
			*t = max;
			return;
		}

		*t += rate;
	}

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
