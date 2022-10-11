#include "Numbers.h"
#include <string>
#include <cstring>
#include <vcclr.h>

std::array<int, 10> Numbers::numbers_;

Numbers::Numbers()
{}

Numbers::~Numbers()
{}

void Numbers::LoadResources()
{
	std::string path = "Resources/Textures/numbers/";
	std::string num;

	for (UINT i = 0; i < MAX_NUM_; i++)
	{
		num = std::to_string(i);
		num += ".png";
		path += num;

		wchar_t *w_path;

		MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			path.c_str(),
			path.size(),
			w_path,
			path.size() + 1
		);

		numbers_[i] = Sprite::LoadTex(w_path);
		//numbers_[i] = Sprite::LoadTex(L"Resources/Textures/numbers/");
	}
}
