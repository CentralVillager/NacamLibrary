#include "Ui.h"

const int Ui::max_char_count_;

Ui::Ui() {
}

void Ui::Initialize() {

	// スプライトの生成
	for (int i = 0; i < max_char_count_; i++) {
		numbers_array_[i] = *Sprite::Create(NUMBER, { 0, 0 });
	}
	for (int i = 0; i < 10; i++) {
		numbers_texture_[i] = *Sprite::Create(NUMBER, { 0, 0 });
	}
	SpritNumberTexture();
}

void Ui::Finalize() {
}

void Ui::Update() {
}

void Ui::Draw() {
}

void Ui::SpritNumberTexture() {
	// 0から10まで
	for (int i = 0; i < 10; i++) {
		int number_index_y = i / line_count_;
		int number_index_x = i % line_count_;

		// 座標計算
		//numbers_texture_[i].SetPosition({ x + width_ * scale * i, y });
		numbers_texture_[i].SetTextureRect({ static_cast<float>(number_index_x) * number_width_,
											static_cast<float>(number_index_y) * number_height_ },
										   { static_cast<float>(number_width_),
										   static_cast<float>(number_height_) });
		numbers_texture_[i].SetSize({ number_width_, number_height_ });
	}
}

void Ui::DrawNumber(const std::string &number, float x, float y, float scale) {
	// 全ての文字について
	for (int i = 0; i < number.size(); i++) {
		// 最大文字数超過
		if (index_ >= max_char_count_) {
			break;
		}

		// 1文字取り出す
		const unsigned char &c_number = number[i];

		// ASCIIオフセット
		//int ascii_num_offset = 48;
		int number_index = c_number - 48;
		if (c_number >= 0x7f) {
			number_index = 0;
		}

		int number_index_y = number_index / line_count_;
		int number_index_x = number_index % line_count_;

		// 座標計算
		numbers_array_[index_].SetPosition({ x + number_width_ * scale * i, y });
		numbers_array_[index_].SetTextureRect({ static_cast<float>(number_index_x) * number_width_,
											static_cast<float>(number_index_y) * number_height_ },
											  { static_cast<float>(number_width_),
											  static_cast<float>(number_height_) });
		numbers_array_[index_].SetSize({ number_width_ * scale, number_height_ * scale });

		// 文字を1つ進める
		index_++;
	}

	// 全ての文字のスプライトについて
	for (int i = 0; i < index_; i++) {
		// 描画
		numbers_array_[i].Draw();
	}

	index_ = 0;
}

void Ui::DrawNumber(const int &number, float x, float y, float scale) {
	// いったん空にする
	numbers_.clear();

	// 数字を桁ごとに分解して逆向きに保存
	for (int tmp = number; tmp > 0;) {
		numbers_.push_back(tmp % 10);
		tmp /= 10;
	}

	// 並べなおす
	std::reverse(numbers_.begin(), numbers_.end());

	// 順番に描画
	for (int i = 0; i < numbers_.size(); i++) {
		numbers_texture_[numbers_[i]].SetPosition({ static_cast<float>(x + number_width_ * scale * i), y });
		numbers_texture_[numbers_[i]].SetSize({ number_width_ * scale, number_height_ * scale });
		numbers_texture_[numbers_[i]].Draw();
	}
}

void Ui::DrawTexture(SpriteType texture_name, XMFLOAT2 position) {
	texture[texture_name].SetPosition(position);
	texture[texture_name].Draw();
}
