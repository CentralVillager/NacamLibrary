#pragma once
#include <string>
#include <array>
#include "../../../Lib/Sprite/NcmSprite.h"

class UltimateManager
{
	enum class UltTex
	{
		u, l, t, i, m, a, T, e, stanby
	};

private:

	static constexpr uint32_t MAX_ULT_TEX_NUM_ = 9;

private:

	uint32_t currect_ult_per_;
	uint32_t max_ult_per_;
	bool is_triggering_;

	static std::array<int, MAX_ULT_TEX_NUM_> tex_hub_;

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
