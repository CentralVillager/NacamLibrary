#pragma once
#include <list>
#include <vector>
#include <array>

// イージング用ハンドル格納型
typedef uint32_t ncm_ehandle;

/// <summary>
/// イージングの種類
/// </summary>
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

/// <summary>
/// イージングの設定構造体
/// </summary>
struct NcmEaseDesc
{
	// 初期値
	float init_value = 0.0f;
	// 総移動量
	float total_move = 0.0f;
	// イージングの仕方
	NcmEaseType ease_type;
	float t_rate;
	float t_max = 1.0f;

	// 識別用ハンドル
	ncm_ehandle handle;
	// 変化量
	float t;
	// イージング関数の戻り値の格納先
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

/// <summary>
/// イージングを管理します。
/// </summary>
class NcmEasing
{
	// イージングに必要なデータ
	static std::vector<NcmEaseDesc> ease_datas_;

	// 関数ポインタ
	typedef float (*EaseFunc)(const float v);

	// イージング関数の格納先
	static std::array<EaseFunc, (size_t)(NcmEaseType::MaxEaseNum)> ease_types_;

	// 総ハンドルカウント変数
	static ncm_ehandle handle_counter_;

public:

	NcmEasing();
	~NcmEasing();
	static void StaticInit();

public:

	/// <summary>
	/// イージングの設定を登録します。
	/// </summary>
	/// <returns>ハンドル</returns>
	static ncm_ehandle RegisterEaseData(const NcmEaseDesc &args);

	/// <summary>
	/// イージングの設定を削除します。
	/// </summary>
	/// <param name="handle">削除したい設定のハンドル</param>
	static void DeleteEaseData(ncm_ehandle handle);

	/// <summary>
	/// 値を更新します。
	/// </summary>
	static void UpdateValue(ncm_ehandle handle);

	/// <summary>
	/// 値をリセットします。
	/// </summary>
	static void ResetTime(ncm_ehandle handle);

	/// <summary>
	/// 現在の値を取得します。
	/// </summary>
	/// <returns>現在の値</returns>
	static float GetValue(ncm_ehandle handle);

	/// <summary>
	/// 遷移が終了したかを返します。
	/// </summary>
	/// <param name="handle"></param>
	/// <returns>終了したか</returns>
	static bool IsFinished(ncm_ehandle handle);

	/// <summary>
	/// 新しく初期値を設定します。
	/// </summary>
	/// <param name="handle">目的のハンドル</param>
	/// <param name="v">設定したい値</param>
	static void SetInitValue(ncm_ehandle handle, float v);

	/// <summary>
	/// 新しく総移動量を設定します。
	/// </summary>
	/// <param name="handle">目的のハンドル</param>
	/// <param name="t">設定したい値</param>
	static void SetTotalMove(ncm_ehandle handle, float v);

private:

	// ハンドルを元に格納されているデータを取得
	static NcmEaseDesc *SearchValue(ncm_ehandle handle);

	// tを更新させる
	static void ConvertRate(float *t, float rate = 0.1f, float max = 1.0f);

private:

	/* 以下遷移率集 */

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
