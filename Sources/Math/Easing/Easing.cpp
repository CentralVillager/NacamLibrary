#include "Easing.h"
#include <cmath>

// 参考サイト
// https://easings.net/ja

const float PI = 3.14159265358979f;

Easing::Easing() {
}

Easing::~Easing() {
}

float Easing::MakeEase(float *param, EaseArgs ease_args) {

	return *param = ease_args.initial_pos + ease_args.total_move * ease_args.variation;
}

float Easing::Lerp(const float &t) {

	return t;
}

float Easing::InSine(const float &t) {

	return 1 - cos(t * PI / 2.0f);
}

float Easing::InCubic(const float &t) {

	return t * t * t;
}

float Easing::InQuint(const float &t) {

	return t * t * t * t * t;
}

float Easing::InCirc(const float &t) {

	return 1 - sqrt(1 - t * t);
}

float Easing::InElastic(const float &t) {

	const float c4 = (2.0f * PI) / 3.0f;

	return t == 0
		? 0
		: t == 1.0f
		? 1
		: -pow(2.0f, 10.0f * t - 10.0f) * sin((t * 10.0f - 10.75f) * c4);
}

float Easing::InQuad(const float &t) {

	return t * t;
}

float Easing::InQuart(const float &t) {

	return t * t * t * t;
}

float Easing::InExpo(const float &t) {

	return t == 0 ? 0 : pow(2.0f, 10.0f * t - 10.0f);
}

float Easing::InBack(const float &t) {

	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}

float Easing::InBounce(const float &t) {

	float temp = 1.0f - t;
	return 1.0f - OutBounce(temp);
}

float Easing::OutSine(const float &t) {

	return sin((t * PI) / 2);
}

float Easing::OutCubic(const float &t) {

	return 1 - pow(1.0f - t, 3.0f);
}

float Easing::OutQuint(const float &t) {

	return 1 - pow(1.0f - t, 5.0f);
}

float Easing::OutCirc(const float &t) {

	return sqrt(1 - (t - 1) * (t - 1));
}

float Easing::OutElastic(const float &t) {

	const float c4 = (2 * PI) / 3;

	return t == 0
		? 0
		: t == 1
		? 1
		: pow(2.0f, -10.0f * t) * sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float Easing::OutQuad(const float &t) {

	return 1 - (1 - t) * (1 - t);
}

float Easing::OutQuart(const float &t) {

	return 1 - pow(1.0f - t, 4.0f);
}

float Easing::OutExpo(const float &t) {

	return 1 - pow(1.0f - t, 4.0f);
}

float Easing::OutBack(const float &t) {

	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1 + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
}

float Easing::OutBounce(float &t) {

	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1) {

		return n1 * t * t;

	} else if (t < 2.0f / d1) {

		return n1 * (t -= 1.5f / d1) * t + 0.75f;

	} else if (t < 2.5f / d1) {

		return n1 * (t -= 2.25f / d1) * t + 0.9375f;

	} else {

		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}

float Easing::InOutSine(const float &t) {

	return -(cos(PI * t) - 1.0f) / 2.0f;
}

float Easing::InOutCubic(const float &t) {

	return t < 0.5f ? 4.0f * t * t * t : 1 - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

float Easing::InOutQuint(const float &t) {

	return t < 0.5f ? 16.0f * t * t * t * t * t : 1 - pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

float Easing::InOutCirc(const float &t) {

	return t < 0.5f
		? (1 - sqrt(1.0f - pow(2.0f * t, 2.0f))) / 2.0f
		: (sqrt(1.0f - pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

float Easing::InOutElastic(const float &t) {

	const float c5 = (2.0f * PI) / 4.5f;

	return t == 0
		? 0
		: t == 1.0f
		? 1.0f
		: t < 0.5f
		? -(pow(2.0f, 20.0f * t - 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f
		: (pow(2.0f, -20.0f * t + 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

float Easing::InOutQuad(const float &t) {

	return t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float Easing::InOutQuart(const float &t) {

	return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
}

float Easing::InOutExpo(const float &t) {

	return t == 0
		? 0
		: t == 1.0f
		? 1.0f
		: t < 0.5f ? pow(2.0f, 20.0f * t - 10.0f) / 2.0f
		: (2.0f - pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

float Easing::InOutBack(const float &t) {

	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return t < 0.5f
		? (pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
		: (pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}
