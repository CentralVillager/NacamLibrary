#include "NcmEasing.h"
#include <cmath>
#include "../Sources/App/Utility/NcmUtil.h"

// �Q�l�T�C�g
// https://easings.net/ja

std::list<EaseArgs> NcmEasing::easing_datas_;
std::array<NcmEasing::EaseFunc, (size_t)(EaseType::MaxEaseNum)> NcmEasing::ease_types_;
int NcmEasing::handle_counter_ = 0;

NcmEasing::NcmEasing()
{}

NcmEasing::~NcmEasing()
{
	easing_datas_.clear();
}

void NcmEasing::StaticInit()
{
	ease_types_[(int)(EaseType::Lerp)] = Lerp;
	ease_types_[(int)(EaseType::InQuad)] = InQuad;
	ease_types_[(int)(EaseType::InCubic)] = InCubic;
	ease_types_[(int)(EaseType::InQuart)] = InQuart;
	ease_types_[(int)(EaseType::InQuint)] = InQuint;
	ease_types_[(int)(EaseType::InSine)] = InSine;
	ease_types_[(int)(EaseType::InCirc)] = InCirc;
	ease_types_[(int)(EaseType::InExpo)] = InExpo;
	ease_types_[(int)(EaseType::InBack)] = InBack;
	ease_types_[(int)(EaseType::InElastic)] = InElastic;
	ease_types_[(int)(EaseType::InBounce)] = InBounce;
	ease_types_[(int)(EaseType::OutQuad)] = OutQuad;
	ease_types_[(int)(EaseType::OutCubic)] = OutCubic;
	ease_types_[(int)(EaseType::OutQuart)] = OutQuart;
	ease_types_[(int)(EaseType::OutQuint)] = OutQuint;
	ease_types_[(int)(EaseType::OutSine)] = OutSine;
	ease_types_[(int)(EaseType::OutCirc)] = OutCirc;
	ease_types_[(int)(EaseType::OutExpo)] = OutExpo;
	ease_types_[(int)(EaseType::OutBack)] = OutBack;
	ease_types_[(int)(EaseType::OutElastic)] = OutElastic;
	ease_types_[(int)(EaseType::OutBounce)] = OutBounce;
}

EaseArgs *NcmEasing::SearchValue(int handle)
{
	for (auto &i : easing_datas_)
	{
		if (i.handle == handle)
		{
			return &i;
		}
	}

	return nullptr;
}

int NcmEasing::RegisterEaseData(const EaseArgs &args)
{
	easing_datas_.emplace_back();
	easing_datas_.back().handle = handle_counter_;
	easing_datas_.back().t = 0;
	easing_datas_.back().init_value = args.init_value;
	easing_datas_.back().total_move = args.total_move;
	easing_datas_.back().ease_type = args.ease_type;

	handle_counter_++;

	return easing_datas_.back().handle;
}

void NcmEasing::UpdateValue(int handle)
{
	// �n���h���ɑΉ�����EaseArgs�������Ă���
	auto args = SearchValue(handle);
	// t���v�Z
	ConvertRate(&args->t, 0.05f, 1.0f);
	// �Y���̃C�[�W���O�֐���K�p
	EaseFunc ease_func = ease_types_[(int)(args->ease_type)];
	// �C�[�W���O�ʂ��v�Z
	args->ease_value = ease_func(args->t);
}

void NcmEasing::ResetTime()
{
	// �S�Ẵf�[�^�ɑ΂���
	for (auto &i : easing_datas_)
	{
		// t�����Z�b�g����
		i.t = 0.0f;
	}
}

void NcmEasing::ResetTime(int handle)
{
	auto args = SearchValue(handle);
	args->t = 0.0f;
}

float NcmEasing::GetValue(int handle)
{
	auto args = SearchValue(handle);
	return args->init_value + args->total_move * args->ease_value;
}

void NcmEasing::SetInitValue(int handle, float v)
{
	auto args = SearchValue(handle);
	args->init_value = v;
}

void NcmEasing::SetTotalMove(int handle, float v)
{
	auto args = SearchValue(handle);
	args->total_move = v;
}

float NcmEasing::MakeEase(float *param, const EaseArgs &ease_args)
{
	return *param = ease_args.init_value + ease_args.total_move * ease_args.ease_value;
}

float NcmEasing::Lerp(const float t)
{
	return t;
}

float NcmEasing::InSine(const float t)
{
	return 1 - cos(t * PI_ / 2.0f);
}

float NcmEasing::InCubic(const float t)
{
	return t * t * t;
}

float NcmEasing::InQuint(const float t)
{
	return t * t * t * t * t;
}

float NcmEasing::InCirc(const float t)
{
	return 1 - sqrt(1 - t * t);
}

float NcmEasing::InElastic(const float t)
{
	const float c4 = (2.0f * PI_) / 3.0f;

	return t == 0
		? 0
		: t == 1.0f
		? 1
		: -pow(2.0f, 10.0f * t - 10.0f) * sin((t * 10.0f - 10.75f) * c4);
}

float NcmEasing::InQuad(const float t)
{
	return t * t;
}

float NcmEasing::InQuart(const float t)
{
	return t * t * t * t;
}

float NcmEasing::InExpo(const float t)
{
	return t == 0 ? 0 : pow(2.0f, 10.0f * t - 10.0f);
}

float NcmEasing::InBack(const float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}

float NcmEasing::InBounce(const float t)
{
	float temp = 1.0f - t;
	return 1.0f - OutBounce(temp);
}

float NcmEasing::OutSine(const float t)
{
	return sin((t * PI_) / 2);
}

float NcmEasing::OutCubic(const float t)
{
	return 1 - pow(1.0f - t, 3.0f);
}

float NcmEasing::OutQuint(const float t)
{
	return 1 - pow(1.0f - t, 5.0f);
}

float NcmEasing::OutCirc(const float t)
{
	return sqrt(1 - (t - 1) * (t - 1));
}

float NcmEasing::OutElastic(const float t)
{
	const float c4 = (2 * PI_) / 3;

	return t == 0
		? 0
		: t == 1
		? 1
		: pow(2.0f, -10.0f * t) * sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float NcmEasing::OutQuad(const float t)
{
	return 1 - (1 - t) * (1 - t);
}

float NcmEasing::OutQuart(const float t)
{
	return 1 - pow(1.0f - t, 4.0f);
}

float NcmEasing::OutExpo(const float t)
{
	return 1 - pow(1.0f - t, 4.0f);
}

float NcmEasing::OutBack(const float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1 + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
}

float NcmEasing::OutBounce(float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1)
	{
		return n1 * t * t;
	}
	else if (t < 2.0f / d1)
	{
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1)
	{
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else
	{
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}

float NcmEasing::InOutSine(const float t)
{
	return -(cos(PI_ * t) - 1.0f) / 2.0f;
}

float NcmEasing::InOutCubic(const float t)
{
	return t < 0.5f ? 4.0f * t * t * t : 1 - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

float NcmEasing::InOutQuint(const float t)
{
	return t < 0.5f ? 16.0f * t * t * t * t * t : 1 - pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

float NcmEasing::InOutCirc(const float t)
{
	return t < 0.5f
		? (1 - sqrt(1.0f - pow(2.0f * t, 2.0f))) / 2.0f
		: (sqrt(1.0f - pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

float NcmEasing::InOutElastic(const float t)
{
	const float c5 = (2.0f * PI_) / 4.5f;

	return t == 0
		? 0
		: t == 1.0f
		? 1.0f
		: t < 0.5f
		? -(pow(2.0f, 20.0f * t - 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f
		: (pow(2.0f, -20.0f * t + 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

float NcmEasing::InOutQuad(const float t)
{
	return t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float NcmEasing::InOutQuart(const float t)
{
	return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
}

float NcmEasing::InOutExpo(const float t)
{
	return t == 0
		? 0
		: t == 1.0f
		? 1.0f
		: t < 0.5f ? pow(2.0f, 20.0f * t - 10.0f) / 2.0f
		: (2.0f - pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

float NcmEasing::InOutBack(const float t)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return t < 0.5f
		? (pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
		: (pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}