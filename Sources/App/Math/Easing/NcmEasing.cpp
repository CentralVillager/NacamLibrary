#include "NcmEasing.h"
#include <cmath>
#include "../Sources/App/Utility/NcmUtil.h"
#include "../Sources/App/Math/NcmMath.h"

// 参考サイト
// https://easings.net/ja

using namespace NcmMath;

std::vector<NcmEaseDesc> NcmEasing::ease_datas_;
std::array<NcmEasing::EaseFunc, (size_t)(NcmEaseType::MaxEaseNum)> NcmEasing::ease_types_;
ncm_ehandle NcmEasing::handle_counter_ = 0;

NcmEasing::NcmEasing()
{}

NcmEasing::~NcmEasing()
{
	ease_datas_.clear();
}

void NcmEasing::StaticInit()
{
	ease_types_[(int)(NcmEaseType::Lerp)] = Lerp;
	ease_types_[(int)(NcmEaseType::InQuad)] = InQuad;
	ease_types_[(int)(NcmEaseType::InCubic)] = InCubic;
	ease_types_[(int)(NcmEaseType::InQuart)] = InQuart;
	ease_types_[(int)(NcmEaseType::InQuint)] = InQuint;
	ease_types_[(int)(NcmEaseType::InSine)] = InSine;
	ease_types_[(int)(NcmEaseType::InCirc)] = InCirc;
	ease_types_[(int)(NcmEaseType::InExpo)] = InExpo;
	ease_types_[(int)(NcmEaseType::InBack)] = InBack;
	ease_types_[(int)(NcmEaseType::InElastic)] = InElastic;
	ease_types_[(int)(NcmEaseType::InBounce)] = InBounce;
	ease_types_[(int)(NcmEaseType::OutQuad)] = OutQuad;
	ease_types_[(int)(NcmEaseType::OutCubic)] = OutCubic;
	ease_types_[(int)(NcmEaseType::OutQuart)] = OutQuart;
	ease_types_[(int)(NcmEaseType::OutQuint)] = OutQuint;
	ease_types_[(int)(NcmEaseType::OutSine)] = OutSine;
	ease_types_[(int)(NcmEaseType::OutCirc)] = OutCirc;
	ease_types_[(int)(NcmEaseType::OutExpo)] = OutExpo;
	ease_types_[(int)(NcmEaseType::OutBack)] = OutBack;
	ease_types_[(int)(NcmEaseType::OutElastic)] = OutElastic;
	ease_types_[(int)(NcmEaseType::OutBounce)] = OutBounce;
}

NcmEaseDesc *NcmEasing::SearchValue(ncm_ehandle handle)
{
	if (&ease_datas_[handle] == nullptr)
	{
		return nullptr;
	}

	return &ease_datas_[handle];
}

ncm_ehandle NcmEasing::RegisterEaseData(const NcmEaseDesc &args)
{
	ease_datas_.emplace_back(NcmEaseDesc());
	ease_datas_.back() = args;
	ease_datas_.back().handle = handle_counter_;

	handle_counter_++;

	return ease_datas_.back().handle;
}

void NcmEasing::DeleteEaseData(ncm_ehandle handle)
{
	auto args = SearchValue(handle);
	ease_datas_.erase(ease_datas_.begin() + handle);
}

void NcmEasing::UpdateValue(ncm_ehandle handle)
{
	// ハンドルに対応するEaseArgsを持ってくる
	auto args = SearchValue(handle);
	// tを計算
	ConvertRate(&args->t, args->t_rate, args->t_max);
	// 該当のイージング関数を適用
	EaseFunc ease_func = ease_types_[(int)(args->ease_type)];
	// イージング量を計算
	args->ease_value = ease_func(args->t);
}

void NcmEasing::ResetTime(ncm_ehandle handle)
{
	auto args = SearchValue(handle);
	args->t = 0.0f;
}

float NcmEasing::GetValue(ncm_ehandle handle)
{
	auto args = SearchValue(handle);
	return args->init_value + args->total_move * args->ease_value;
}

bool NcmEasing::IsFinished(ncm_ehandle handle)
{
	auto args = SearchValue(handle);

	if (args->t >= 1.0f)
	{
		return true;
	}

	return false;
}

void NcmEasing::SetInitValue(ncm_ehandle handle, float v)
{
	auto args = SearchValue(handle);
	args->init_value = v;
}

void NcmEasing::SetTotalMove(ncm_ehandle handle, float v)
{
	auto args = SearchValue(handle);
	args->total_move = v;
}

void NcmEasing::ConvertRate(float *t, float rate, float max)
{
	if (*t >= max)
	{
		*t = max;
		return;
	}

	*t += rate;
}

float NcmEasing::Lerp(const float t)
{
	return t;
}

float NcmEasing::InSine(const float t)
{
	return 1 - cos(t * PI / 2.0f);
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
	const float c4 = (2.0f * PI) / 3.0f;

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
	return sin((t * PI) / 2);
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
	const float c4 = (2 * PI) / 3;

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
	return -(cos(PI * t) - 1.0f) / 2.0f;
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
	const float c5 = (2.0f * PI) / 4.5f;

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
