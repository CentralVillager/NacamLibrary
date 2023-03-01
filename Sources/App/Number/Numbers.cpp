#include "Numbers.h"
#include <string>
#include <cstring>
#include <algorithm>
#include "../DebugUtill/NcmImGui.h"
#include "../../Lib/Win32App/Win32App.h"

std::array<int, Numbers::MAX_NUM_> Numbers::numbers_;
std::array<int, Numbers::MAX_NUM_> Numbers::dupli_numbers_;

Numbers::Numbers() :
	dead_line_size_(30.0f),
	offset_({ 30.0f, 30.0f }),
	tracking_(),
	digit_width_()
{}

Numbers::~Numbers()
{}

void Numbers::LoadResources()
{
	numbers_[0] = NcmSprite::LoadTex("Resources/Textures/numbers/0.png");
	numbers_[1] = NcmSprite::LoadTex("Resources/Textures/numbers/1.png");
	numbers_[2] = NcmSprite::LoadTex("Resources/Textures/numbers/2.png");
	numbers_[3] = NcmSprite::LoadTex("Resources/Textures/numbers/3.png");
	numbers_[4] = NcmSprite::LoadTex("Resources/Textures/numbers/4.png");
	numbers_[5] = NcmSprite::LoadTex("Resources/Textures/numbers/5.png");
	numbers_[6] = NcmSprite::LoadTex("Resources/Textures/numbers/6.png");
	numbers_[7] = NcmSprite::LoadTex("Resources/Textures/numbers/7.png");
	numbers_[8] = NcmSprite::LoadTex("Resources/Textures/numbers/8.png");
	numbers_[9] = NcmSprite::LoadTex("Resources/Textures/numbers/9.png");

	dupli_numbers_[0] = NcmSprite::LoadTex("Resources/Textures/numbers/0.png");
	dupli_numbers_[1] = NcmSprite::LoadTex("Resources/Textures/numbers/1.png");
	dupli_numbers_[2] = NcmSprite::LoadTex("Resources/Textures/numbers/2.png");
	dupli_numbers_[3] = NcmSprite::LoadTex("Resources/Textures/numbers/3.png");
	dupli_numbers_[4] = NcmSprite::LoadTex("Resources/Textures/numbers/4.png");
	dupli_numbers_[5] = NcmSprite::LoadTex("Resources/Textures/numbers/5.png");
	dupli_numbers_[6] = NcmSprite::LoadTex("Resources/Textures/numbers/6.png");
	dupli_numbers_[7] = NcmSprite::LoadTex("Resources/Textures/numbers/7.png");
	dupli_numbers_[8] = NcmSprite::LoadTex("Resources/Textures/numbers/8.png");
	dupli_numbers_[9] = NcmSprite::LoadTex("Resources/Textures/numbers/9.png");
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

		//numbers_[i] = NcmSprite::LoadTex(w_path);
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

void Numbers::DrawNumber(int number, float scale, float alpha, HorizontalAlignment h_align, VerticalAlignment v_align)
{
	using enum HorizontalAlignment;
	using enum VerticalAlignment;

	// �����������ƂɊi�[���邽�߂̃R���e�i
	std::vector<int> digit;

	bool is_done = false;
	bool is_hundred = false;

	// �����Ƃɕ���
	bool is_unique = DivDigit(&digit, number);

	// �����ꏊ�ɂ���Ĕz���������ύX
	SortDigitWithAlign(&digit, h_align);

	if (digit.size() == 3) { is_hundred = true; }

	if (h_align == Left)
	{
		float size_stack = 0.0f;

		for (UINT i = 0; i < digit.size(); i++)
		{
			NcmSprite::SetScale(numbers_[digit[i]], scale);

			// �����e�N�X�`���̃T�C�Y���擾
			XMFLOAT2 size = NcmSprite::GetSize(numbers_[digit[i]]);

			XMFLOAT2 final_pos{};
			final_pos.x = size_stack;
			final_pos.y = Win32App::SIZE_.y - size.y;

			NcmSprite::DrawTex(numbers_[digit[i]],
				{ (final_pos.x + (i * tracking_)) + dead_line_size_, final_pos.y + dead_line_size_ });

			size_stack += size.x;

			if (!is_unique && !is_done)
			{
				NcmSprite::DrawTex(dupli_numbers_[digit[i]],
					{ (final_pos.x + (i * tracking_)) + dead_line_size_, final_pos.y + dead_line_size_ });
				is_done = true;
			}
		}
	}

	if (h_align == Center)
	{
		digit_width_ = 0;

		// �`�悵�������̍��v�T�C�Y���擾
		for (UINT i = 0; i < digit.size(); i++)
		{
			NcmSprite::SetScale(numbers_[digit[i]], scale);
			NcmSprite::SetScale(dupli_numbers_[digit[i]], scale);
			digit_width_ += NcmSprite::GetSize(numbers_[digit[i]]).x;
		}

		// �`��J�n�ʒu���Z�o
		float start_draw_point = Win32App::FCENTER_.x - (digit_width_ / 2);
		float size_stack = start_draw_point;

		for (UINT i = 0; i < digit.size(); i++)
		{
			// �����e�N�X�`���̃T�C�Y���擾
			XMFLOAT2 size = NcmSprite::GetSize(numbers_[digit[i]]);
			NcmSprite::SetAnchorPoint(numbers_[digit[i]], { 0, 1.0f });
			NcmSprite::SetAnchorPoint(dupli_numbers_[digit[i]], { 0, 1.0f });

			XMFLOAT2 final_pos{};
			final_pos.x = size_stack;
			final_pos.y = Win32App::SIZE_.y - dead_line_size_;

			// �����x��ύX
			NcmSprite::SetAlpha(numbers_[digit[i]], alpha);

			// �`��
			NcmSprite::DrawTex(numbers_[digit[i]],
				{ (final_pos.x + (i * tracking_)), final_pos.y });

			size_stack += size.x;

			// �d�����̉��}���u
			if (!is_unique && !is_done)
			{
				if (!is_hundred)
				{
					// �����x��ύX
					NcmSprite::SetAlpha(dupli_numbers_[digit[i]], alpha);

					NcmSprite::DrawTex(dupli_numbers_[digit[i]],
						{ (final_pos.x + (i * tracking_)), final_pos.y });
					is_done = true;
					is_hundred = true;
				}

				if (is_hundred)
				{
					is_hundred = false;
				}
			}
		}
	}

	if (h_align == Right)
	{
		float size_stack = (float)(Win32App::SIZE_.x);

		for (UINT i = 0; i < digit.size(); i++)
		{
			NcmSprite::SetScale(numbers_[digit[i]], scale);

			// �����e�N�X�`���̃T�C�Y���擾
			XMFLOAT2 size = NcmSprite::GetSize(numbers_[digit[i]]);

			size_stack -= size.x;

			XMFLOAT2 final_pos{};
			final_pos.x = size_stack;
			final_pos.y = Win32App::SIZE_.y - size.y;

			NcmSprite::DrawTex(numbers_[digit[i]],
				{ (final_pos.x - (i * tracking_)) - dead_line_size_, final_pos.y - dead_line_size_ });
		}
	}
}

bool Numbers::DivDigit(std::vector<int> *dist, const int num)
{
	for (int tmp = num; tmp > 0;)
	{
		// �E���猅���ƂɊi�[
		dist->emplace_back(tmp % 10);

		// �ʂ��グ��
		tmp /= 10;
	}

	// ���̌��̐�����0�Ȃ�
	if (num == 0)
	{
		// 0������
		dist->emplace_back(0);
	}

	// �d���m�F�p
	std::vector<int> temp = *dist;
	// �d���͈͂̃C�e���[�^���擾
	auto itr = std::unique(temp.begin(), temp.end());
	// ���͈̔͂��폜
	temp.erase(itr, temp.end());

	// �폜�O�ƌ���r
	return std::equal(dist->begin(), dist->end(), temp.begin(), temp.end());
}

void Numbers::SortDigitWithAlign(std::vector<int> *dist, const HorizontalAlignment &align)
{
	using enum HorizontalAlignment;

	// ������ || ���������Ȃ�
	if (align == Left || align == Center)
	{
		// ���т����R�ɂȂ�悤�ɕϊ�
		std::reverse(dist->begin(), dist->end());
	}
	// ����ȊO�Ȃ�
	else
	{
		// �������Ȃ�
	}
}
