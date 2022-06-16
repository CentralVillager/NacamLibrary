#pragma once
#include "Sprite.h"
#include <DirectXMath.h>

class Ui {
	using XMFLOAT2 = DirectX::XMFLOAT2;

	static const int max_char_count_ = 256;	// 1�t���[�����ł̍ő�\��������
	static const int number_width_ = 41;	// �摜��1�������̉���
	static const int number_height_ = 83;	// �摜��1�������̏c��
	static const int line_count_ = 5;		// �摜��1�������̕�����

	// �����z��
	Sprite numbers_texture_[10];
	std::vector<int> numbers_;

	// ����͎g��Ȃ��ł��B
	Sprite numbers_array_[max_char_count_];

	// �Y����
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
	/// �ǂݍ��񂾐����e�N�X�`���𐔎����Ƃɕ������ĕۑ����܂��B
	/// </summary>
	void SpritNumberTexture();

	/// <summary>
	/// �̂��ɒǉ������ł��낤�f�o�b�O�t�H���g�p
	/// </summary>
	/// <param name="number"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="scale"></param>
	void DrawNumber(const std::string &number, float x, float y, float scale);

	/// <summary>
	/// ���͂��ꂽ������`�悵�܂��B
	/// </summary>
	/// <param name="number">�`�悵��������</param>
	/// <param name="x">x���W</param>
	/// <param name="y">y���W</param>
	/// <param name="scale">�\���X�P�[��</param>
	void DrawNumber(const int &number, float x, float y, float scale);

	/// <summary>
	/// �w�肵���e�N�X�`����`�悵�܂��B
	/// </summary>
	/// <param name="texture_name"></param>
	/// <param name="position"></param>
	void DrawTexture(SpriteType texture_name, XMFLOAT2 position);
};
