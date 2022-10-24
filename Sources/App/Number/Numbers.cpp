#include "Numbers.h"
#include <string>
#include <cstring>
#include "../Debug/NcmImGui.h"
#include "../../Lib/Win32App/Win32App.h"

std::array<int, Numbers::MAX_NUM_> Numbers::numbers_;

Numbers::Numbers() :
	offset_(),
	tracking_()
{}

Numbers::~Numbers()
{}

void Numbers::LoadResources()
{
	numbers_[0] = NcmSprite::LoadTex(L"Resources/Textures/numbers/0.png");
	numbers_[1] = NcmSprite::LoadTex(L"Resources/Textures/numbers/1.png");
	numbers_[2] = NcmSprite::LoadTex(L"Resources/Textures/numbers/2.png");
	numbers_[3] = NcmSprite::LoadTex(L"Resources/Textures/numbers/3.png");
	numbers_[4] = NcmSprite::LoadTex(L"Resources/Textures/numbers/4.png");
	numbers_[5] = NcmSprite::LoadTex(L"Resources/Textures/numbers/5.png");
	numbers_[6] = NcmSprite::LoadTex(L"Resources/Textures/numbers/6.png");
	numbers_[7] = NcmSprite::LoadTex(L"Resources/Textures/numbers/7.png");
	numbers_[8] = NcmSprite::LoadTex(L"Resources/Textures/numbers/8.png");
	numbers_[9] = NcmSprite::LoadTex(L"Resources/Textures/numbers/9.png");
}

void Numbers::LoadNumbers()
{
	std::string path = "Resources/Textures/numbers/";
	std::string num;

	for (UINT i = 0; i < MAX_NUM_; i++)
	{
		num = std::to_string(i);
		num += ".png";
		path += num;

		wchar_t *w_path = nullptr;

		int ret = MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			path.c_str(),
			-1,
			w_path,
			MultiByteToWideChar(
				CP_ACP,
				MB_PRECOMPOSED,
				path.c_str(),
				-1,
				w_path,
				0)
		);

		DWORD r = GetLastError();

		numbers_[i] = NcmSprite::LoadTex(w_path);
	}
}

void Numbers::Initialize()
{
	offset_ = { 30.0f, 30.0f };
	tracking_ = 0.0f;
}

void Numbers::Draw()
{}

void Numbers::DebugDraw()
{
	ImGui::Dummy(ImVec2(10.0f, 10.0f));
	ImGui::Text("Texture Config");

	NcmImGui::DragFloat2("Offset", offset_, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("Tracking", &tracking_, 0.1f, -100.0f, 100.0f);
}

void Numbers::DrawNumber(int number, float scale, HorizontalAlignment h_align, VerticalAlignment v_align)
{
	using enum HorizontalAlignment;
	using enum VerticalAlignment;

	// 数字を桁ごとに格納するためのコンテナ
	std::vector<int> digit;

	// 桁ごとに分割
	DivDigit(&digit, number);

	// 揃え場所によって配列内順序を変更
	SortDigitWithAlign(&digit, h_align);

	if (h_align == Left)
	{
		float size_stack = 0.0f;

		for (UINT i = 0; i < digit.size(); i++)
		{
			NcmSprite::SetScale(numbers_[digit[i]], scale);

			// 数字テクスチャのサイズを取得
			XMFLOAT2 size = NcmSprite::GetSize(numbers_[digit[i]]);

			XMFLOAT2 final_pos{};
			final_pos.x = size_stack;
			final_pos.y = Win32App::SIZE_.y - size.y;

			NcmSprite::DrawTex(numbers_[digit[i]],
				{ (final_pos.x + (i * tracking_)) + offset_.x, final_pos.y + offset_.y });

			size_stack += size.x;

			NcmSprite::ResetScale(numbers_[digit[i]], scale);
		}
	}

	if (h_align == Right)
	{
		float size_stack = (float)(Win32App::SIZE_.x);

		for (UINT i = 0; i < digit.size(); i++)
		{
			NcmSprite::SetScale(numbers_[digit[i]], scale);

			// 数字テクスチャのサイズを取得
			XMFLOAT2 size = NcmSprite::GetSize(numbers_[digit[i]]);

			size_stack -= size.x;

			XMFLOAT2 final_pos{};
			final_pos.x = size_stack;
			final_pos.y = Win32App::SIZE_.y - size.y;

			NcmSprite::DrawTex(numbers_[digit[i]],
				{ (final_pos.x - (i * tracking_)) - offset_.x, final_pos.y - offset_.y });

			NcmSprite::ResetScale(numbers_[digit[i]], scale);
		}
	}
}

void Numbers::DivDigit(std::vector<int> *dist, const int num)
{
	for (int tmp = num; tmp > 0;)
	{
		// 右から桁ごとに格納
		dist->push_back(tmp % 10);

		// 位を上げる
		tmp /= 10;
	}

	if (num == 0)
	{
		dist->push_back(0);
	}
}

void Numbers::SortDigitWithAlign(std::vector<int> *dist, const HorizontalAlignment &align)
{
	using enum HorizontalAlignment;

	// 左揃えなら
	if (align == Left)
	{
		// 並びが自然になるように変換
		std::reverse(dist->begin(), dist->end());
	}
	// それ以外なら
	else
	{
		// 何もしない
	}
}
