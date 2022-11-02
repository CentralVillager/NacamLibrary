#pragma once
#include <string>
#include <array>
#include <memory>
#include "../../../Lib/Sprite/NcmSprite.h"
#include "../../Number/Numbers.h"
#include "../Sources/App/Math/Easing/NcmEasing.h"

class UltimateManager
{
	enum class UltTex
	{
		u, l, t, i, m, a, T, e, stanby
	};

	enum class TexName
	{
		percent
	};

private:

	static constexpr uint32_t MAX_ULT_TEX_NUM_ = 9;

private:

	static std::array<int, MAX_ULT_TEX_NUM_> ult_tex_hub_;
	static ncm_thandle percent_;

	uint32_t currect_ult_per_;
	uint32_t max_ult_per_;
	uint32_t value_dist_;
	bool is_triggering_;

	ncm_ehandle ease_;
	bool is_change_;

	std::unique_ptr<Numbers> nums_;

	float ImGui_num_scale_ = 0.4f;

public:

	UltimateManager();
	~UltimateManager();

public:

	static void LoadResources();

	void Initialize();
	void Update();
	void Draw();
	void DrawUi();
	void DebugDraw();

public:

	void AddUltValue(uint32_t v);
	bool NoticeFullCharged();
	void TriggeringUlt();
};
