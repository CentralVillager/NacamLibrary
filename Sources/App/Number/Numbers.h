#pragma once
#include <array>
#include <vector>
#include <DirectXMath.h>
#include "../../Lib/Sprite/Sprite.h"

enum class HorizontalAlignment
{
	Left,
	Right
};

enum class VerticalAlignment
{
	Top,
	Center,
	Bottom
};

class Numbers
{
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	Numbers();
	~Numbers();

private:

	static constexpr UINT MAX_NUM_ = 10;
	static std::array<int, MAX_NUM_> numbers_;

	XMFLOAT2 offset_;
	float tracking_;

public:

	static void LoadResources();
	static void LoadNumbers();

public:

	void Initialize();
	void Draw();
	void DebugDraw();

public:

	void DrawNumber(int number, float scale, HorizontalAlignment h_align, VerticalAlignment v_align);

private:

	void DivDigit(std::vector<int> *dist, const int &num);
	void SortDigitWithAlign(std::vector<int> *dist, const HorizontalAlignment &align);
};
