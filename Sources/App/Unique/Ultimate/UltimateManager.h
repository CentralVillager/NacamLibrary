#pragma once
#include <string>
#include <array>
#include <memory>
#include <DirectXMath.h>
#include "../../../Lib/Sprite/NcmSprite.h"
#include "../../Number/Numbers.h"
#include "../Sources/App/Math/Easing/NcmEasing.h"

/// <summary>
/// アルティメットに関するクラス
/// </summary>
class UltimateManager
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:

	enum class UltTex
	{
		u, l, t, i, m, a, T, e, stanby
	};

	enum class TexName
	{
		percent
	};

private:

	// 最大ウルトテクスチャ数
	static constexpr uint32_t MAX_ULT_TEX_NUM_ = 9;

	static constexpr float MAX_ALPHA_VALUE_ = 1.0f;	// 最大α値
	static constexpr float MIN_ALPHA_VALUE_ = 0.3f;	// 最小α値

private:

	// ウルトテクスチャのハンドル格納コンテナ
	static std::array<ncm_thandle, MAX_ULT_TEX_NUM_> ult_tex_hub_;
	static ncm_thandle percent_;
	static ncm_thandle icon_q_;

private:

	uint32_t currect_ult_per_;	// 現ウルト％
	uint32_t max_ult_per_;		// 最大ウルト％
	uint32_t value_dist_;		// イージング用目標値
	bool is_triggering_;		// 発動したか
	float num_alpha_;			// %のα値

	ncm_ehandle ease_;			// イージングハンドル
	ncm_ehandle num_alpha_to_max_;	// %表示用イージングハンドル
	ncm_ehandle num_alpha_to_min_;	// %表示用イージングハンドル
	bool to_max_;				// イージングの値がmaxの値に向けて遷移中か
	bool to_min_;				// イージングの値がmaxの値に向けて遷移中か
	bool is_change_;			// ウルトの値が更新されたら

	std::unique_ptr<Numbers> nums_;	// 数字

	uint32_t ult_tex_draw_intervel_;// ウルト演出の描画インターバル

	bool is_finish_display_;	// ウルト演出が終了したか

	float ImGui_num_scale_;
	float ImGui_offset_x_;
	float ImGui_offset_all_y_;
	float ImGui_tex_scale_;
	float ImGui_tex_alpha_;

public:

	UltimateManager();
	~UltimateManager();

public:

	static void LoadResources();

	void Initialize();
	void Update();
	void Draw();
	void DebugDraw();

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUi();

	/// <summary>
	/// ウルト演出の描画
	/// </summary>
	void DrawUltimateStanby();

public:

	/// <summary>
	/// ウルト％を加算する
	/// </summary>
	/// <param name="v"></param>
	void AddUltValue(uint32_t v);

	/// <summary>
	/// 最大まで溜まったことを通知する
	/// </summary>
	/// <returns></returns>
	bool NoticeFullCharged();

	/// <summary>
	/// ウルトを発動する
	/// </summary>
	void TriggeringUlt();

private:

	bool DrawULTIMATESequence();

	bool DrawSTANBYSequence();

	bool CheckDoDisplay();
};
