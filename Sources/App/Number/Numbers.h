#pragma once
#include <array>
#include <vector>
#include <DirectXMath.h>
#include "../../Lib/Sprite/NcmSprite.h"

enum class HorizontalAlignment
{
	Left, Center, Right
};

enum class VerticalAlignment
{
	Top,
	Middle,
	Bottom
};

class Numbers
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:

	Numbers();
	~Numbers();

private:

	static constexpr UINT MAX_NUM_ = 10;
	static std::array<int, MAX_NUM_> numbers_;
	static std::array<int, MAX_NUM_> dupli_numbers_;

	float dead_line_size_;
	XMFLOAT2 offset_;
	float tracking_;

	float digit_width_;

public:

	static void LoadResources();
	static void LoadNumbers();

public:

	void Initialize();
	void Draw();
	void DebugDraw();

public:

	void DrawNumber(int number, float scale, float alpha, HorizontalAlignment h_align, VerticalAlignment v_align);

	float GetDigitsWidth() { return digit_width_; }
	float GetDeadLineSize() { return dead_line_size_; }

private:

	bool DivDigit(std::vector<int> *dist, const int num);
	void SortDigitWithAlign(std::vector<int> *dist, const HorizontalAlignment &align);
};
