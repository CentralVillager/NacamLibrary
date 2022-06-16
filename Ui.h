#pragma once
#include "Sprite.h"
#include <DirectXMath.h>

class Ui {
	using XMFLOAT2 = DirectX::XMFLOAT2;

	static const int max_char_count_ = 256;	// 1フレーム内での最大表示文字数
	static const int number_width_ = 41;	// 画像内1文字分の横幅
	static const int number_height_ = 83;	// 画像内1文字分の縦幅
	static const int line_count_ = 5;		// 画像内1文字分の文字数

	// 数字配列
	Sprite numbers_texture_[10];
	std::vector<int> numbers_;

	// 今回は使わないです。
	Sprite numbers_array_[max_char_count_];

	// 添え字
	int index_ = 0;

	Sprite texture[13];

public:
	enum SpriteType {
		NUMBER
	};

	Ui();

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	/// <summary>
	/// 読み込んだ数字テクスチャを数字ごとに分割して保存します。
	/// </summary>
	void SpritNumberTexture();

	/// <summary>
	/// のちに追加されるであろうデバッグフォント用
	/// </summary>
	/// <param name="number"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="scale"></param>
	void DrawNumber(const std::string &number, float x, float y, float scale);

	/// <summary>
	/// 入力された数字を描画します。
	/// </summary>
	/// <param name="number">描画したい数字</param>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	/// <param name="scale">表示スケール</param>
	void DrawNumber(const int &number, float x, float y, float scale);

	/// <summary>
	/// 指定したテクスチャを描画します。
	/// </summary>
	/// <param name="texture_name"></param>
	/// <param name="position"></param>
	void DrawTexture(SpriteType texture_name, XMFLOAT2 position);
};
