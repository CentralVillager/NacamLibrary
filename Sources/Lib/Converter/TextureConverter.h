#pragma once
#include <string>
#include <DirectXTex.h>

class TextureConverter
{
private:

	DirectX::TexMetadata metadata_;			// �摜�̏��
	DirectX::ScratchImage scratch_image_;	// �摜�C���[�W�̃R���e�i

	std::wstring directory_path_;	// �f�B���N�g���p�X
	std::wstring file_name_;		// �t�@�C����
	std::wstring file_ext_;			// �t�@�C���g���q

public:

	/// <summary>
	/// WIC����DDS�`���ɕϊ�����
	/// </summary>
	/// <param name="path"></param>
	void ConvertTextureWICToDDS(const std::string &path);

private:

	/// <summary>
	/// �e�N�X�`���t�@�C���ǂݍ���
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	void LoadWICTextureFromFile(const std::string &path);

	/// <summary>
	/// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	void SeparateFilePath(const std::wstring &path);

	/// <summary>
	/// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	/// </summary>
	void SaveDDSTextureToFile();

public:

	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="m_string"></param>
	/// <returns></returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string &m_string);

	/// <summary>
	/// �t�@�C���g���q��Ԃ�
	/// </summary>
	/// <param name="m_string"></param>
	/// <returns></returns>
	static std::wstring ReturnFileExtention(const std::wstring &path);
};
