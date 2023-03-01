#include "TextureConverter.h"
#include <Windows.h>

using namespace std;
using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string &path)
{
	// �e�N�X�`���t�@�C����ǂݍ���
	LoadWICTextureFromFile(path);

	// DDS�`���֕ϊ�����
	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string &path)
{
	HRESULT result;

	// �t�@�C���p�X�����C�h������֕ϊ�
	wstring w_file_path = ConvertMultiByteStringToWideString(path);

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(w_file_path.c_str(), WIC_FLAGS_NONE, &metadata_, scratch_image_);
	assert(SUCCEEDED(result));

	// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	SeparateFilePath(w_file_path);
}

void TextureConverter::SeparateFilePath(const std::wstring &path)
{
	size_t pos_1;
	wstring except_ext;

	// ��؂蕶�� '.' ���o�Ă����ԍŌ�̕���������
	pos_1 = path.rfind('.');

	// �������q�b�g������
	if (pos_1 != wstring::npos)
	{
		// ��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		file_ext_ = path.substr(pos_1 + 1, path.size() - pos_1 - 1);
		// ��؂蕶���̑O�܂ł𔲂��o��
		except_ext = path.substr(0, pos_1);
	}
	else
	{
		file_ext_ = L"";
		except_ext = path;
	}

	// ��؂蕶�� '\\' ���o�Ă����ԍŌ�̕���������
	pos_1 = except_ext.rfind('\\');

	// �������q�b�g������
	if (pos_1 != wstring::npos)
	{
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directory_path_ = except_ext.substr(0, pos_1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		file_name_ = except_ext.substr(pos_1 + 1, except_ext.size() - pos_1 - 1);

		return;
	}

	// ��؂蕶�� '/' ���o�Ă����ԍŌ�̕���������
	pos_1 = except_ext.rfind('/');

	// �������q�b�g������
	if (pos_1 != wstring::npos)
	{
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directory_path_ = except_ext.substr(0, pos_1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		file_name_ = except_ext.substr(pos_1 + 1, except_ext.size() - pos_1 - 1);

		return;
	}

	// ��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
	directory_path_ = L"";
	file_name_ = except_ext;
}

void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT result;

	// �o�̓t�@�C������ݒ肷��
	wstring file_path = directory_path_ + file_name_ + L".dds";

	// DDS�t�@�C�������o��
	result = SaveToDDSFile(scratch_image_.GetImages(), scratch_image_.GetImageCount(), metadata_,
		DDS_FLAGS_NONE, file_path.c_str());
	assert(SUCCEEDED(result));

	// �ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata_.format = MakeSRGB(metadata_.format);
}

wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string &m_string)
{
	// ���������v�Z
	int path_size = MultiByteToWideChar(CP_ACP, 0, m_string.c_str(), -1, nullptr, 0);

	wstring w_string;
	w_string.resize(path_size);

	// ���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, m_string.c_str(), -1, &w_string[0], path_size);

	return w_string;
}

std::wstring TextureConverter::ReturnFileExtention(const std::wstring &path)
{
	size_t pos_1;

	// ��؂蕶�� '.' ���o�Ă����ԍŌ�̕���������
	pos_1 = path.rfind('.');

	// �������q�b�g������
	if (pos_1 != wstring::npos)
	{
		// ��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		return path.substr(pos_1 + 1, path.size() - pos_1 - 1);
	}
	else
	{
		return L"";
	}
}
