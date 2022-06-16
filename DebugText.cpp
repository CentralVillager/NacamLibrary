#include "DebugText.h"

void DebugText::Initialize() {
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++) {
		// �X�v���C�g�𐶐�����
		sprites[i] = *Sprite::Create(0, { 0, 0 });
	}
}

void DebugText::Print(const std::string &text, float x, float y, float scale) {
	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++) {
		// �ő啶��������
		if (sprite_index_ >= max_char_count_) {
			break;
		}
		// 1�������o��(ASCII�̂ݐ��藧��)
		const unsigned char &character = text[i];

		// ASCII�R�[�h��2�i����΂����ԍ����v�Z
		int font_index = character - 32;
		if (character >= 0x7f) {
			font_index = 0;
		}

		int font_index_y = font_index / font_line_count_;
		int font_index_x = font_index % font_line_count_;

		// ���W�v�Z
		sprites[sprite_index_].SetPosition({ x + font_width_ * scale * i, y });
		sprites[sprite_index_].SetTextureRect({ static_cast<float>(font_index_x) * font_width_, static_cast<float>(font_index_y) * font_height_ },
											  { font_width_ * scale, font_height_ * scale });
		sprites[sprite_index_].SetSize({ font_width_ * scale, font_height_ * scale });

		sprite_index_++;
	}
}

void DebugText::DrawAll() {
	for (int i = 0; i < sprite_index_; i++) {
		sprites[i].Draw();
	}

	sprite_index_ = 0;
}
