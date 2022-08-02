#pragma once

enum class EaseType {

	Lerp,

	InSmooth,
	InQuad,
	InCubic,
	InQuart,
	InQuint,
	InSine,
	InCirc,
	InExpo,
	InBack,
	InSostBack,
	InElastic,
	InBounce,

	OutSmooth,
	OutQuad,
	OutCubic,
	OutQuart,
	OutnQuint,
	OutSine,
	OutCirc,
	OutExpo,
	OutBack,
	OutSostBack,
	OutElastic,
	OutBounce,
};

struct EaseArgs {

	float initial_pos = 0.0f;
	float total_move = 0.0f;
	float variation = 0.0f;
};

class Easing {

	float t_ = 0.0f;

public:

	Easing();
	~Easing();

public:

	static float MakeEase(float *param, EaseArgs ease_args);
	static inline void ConvertRate(float *t, float rate = 0.1f, float max = 1.0f) {

		if (*t >= max) {

			*t = max;
			return;
		}

		*t += rate;
	}

public:

	static float Lerp(const float &t);

	static float InSine(const float &t);
	static float InCubic(const float &t);
	static float InQuint(const float &t);
	static float InCirc(const float &t);
	static float InElastic(const float &t);
	static float InQuad(const float &t);
	static float InQuart(const float &t);
	static float InExpo(const float &t);
	static float InBack(const float &t);
	static float InBounce(const float &t);

	static float OutSine(const float &t);
	static float OutCubic(const float &t);
	static float OutQuint(const float &t);
	static float OutCirc(const float &t);
	static float OutElastic(const float &t);
	static float OutQuad(const float &t);
	static float OutQuart(const float &t);
	static float OutExpo(const float &t);
	static float OutBack(const float &t);
	static float OutBounce(float &t);

	static float InOutSine(const float &t);
	static float InOutCubic(const float &t);
	static float InOutQuint(const float &t);
	static float InOutCirc(const float &t);
	static float InOutElastic(const float &t);
	static float InOutQuad(const float &t);
	static float InOutQuart(const float &t);
	static float InOutExpo(const float &t);
	static float InOutBack(const float &t);
};
