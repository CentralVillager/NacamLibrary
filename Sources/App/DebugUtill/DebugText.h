#pragma once
#include <string>
#include "../../Lib/2D/NcmSprite.h"

class DebugText
{
	static const int max_char_count_ = 256;
	static const int font_width_ = 9;
	static const int font_height_ = 18;
	static const int font_line_count_ = 14;

	// スプライトの配列
	//Sprite sprites[max_char_count_];
	int sprites_[max_char_count_];

	// スプライト配列の添え次番号
	int sprite_index_ = 0;

public:
	void Initialize();

	void Print(const std::string &text, float x, float y, float scale = 1.0f);

	void DrawAll();
};
