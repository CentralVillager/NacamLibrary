#pragma once
#include <array>
#include "../Sprite/Sprite.h"

class Numbers
{
public:

	Numbers();
	~Numbers();

private:

	static const UINT MAX_NUM_ = 10;

private:

	static std::array<int, 10> numbers_;

public:

	static void LoadResources();
};
