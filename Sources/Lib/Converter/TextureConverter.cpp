#include "TextureConverter.h"
#include <Windows.h>

using namespace std;
using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string &path)
{
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(path);

	// DDS形式へ変換する
	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string &path)
{
	HRESULT result;

	// ファイルパスをワイド文字列へ変換
	wstring w_file_path = ConvertMultiByteStringToWideString(path);

	// WICテクスチャのロード
	result = LoadFromWICFile(w_file_path.c_str(), WIC_FLAGS_NONE, &metadata_, scratch_image_);
	assert(SUCCEEDED(result));

	// フォルダパスとファイル名を分離する
	SeparateFilePath(w_file_path);
}

void TextureConverter::SeparateFilePath(const std::wstring &path)
{
	size_t pos_1;
	wstring except_ext;

	// 区切り文字 '.' が出てくる一番最後の部分を検索
	pos_1 = path.rfind('.');

	// 検索がヒットしたら
	if (pos_1 != wstring::npos)
	{
		// 区切り文字の後ろをファイル拡張子として保存
		file_ext_ = path.substr(pos_1 + 1, path.size() - pos_1 - 1);
		// 区切り文字の前までを抜き出す
		except_ext = path.substr(0, pos_1);
	}
	else
	{
		file_ext_ = L"";
		except_ext = path;
	}

	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos_1 = except_ext.rfind('\\');

	// 検索がヒットしたら
	if (pos_1 != wstring::npos)
	{
		// 区切り文字の前までをディレクトリパスとして保存
		directory_path_ = except_ext.substr(0, pos_1 + 1);
		// 区切り文字の後ろをファイル名として保存
		file_name_ = except_ext.substr(pos_1 + 1, except_ext.size() - pos_1 - 1);

		return;
	}

	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos_1 = except_ext.rfind('/');

	// 検索がヒットしたら
	if (pos_1 != wstring::npos)
	{
		// 区切り文字の前までをディレクトリパスとして保存
		directory_path_ = except_ext.substr(0, pos_1 + 1);
		// 区切り文字の後ろをファイル名として保存
		file_name_ = except_ext.substr(pos_1 + 1, except_ext.size() - pos_1 - 1);

		return;
	}

	// 区切り文字がないのでファイル名のみとして扱う
	directory_path_ = L"";
	file_name_ = except_ext;
}

void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT result;

	// 出力ファイル名を設定する
	wstring file_path = directory_path_ + file_name_ + L".dds";

	// DDSファイル書き出し
	result = SaveToDDSFile(scratch_image_.GetImages(), scratch_image_.GetImageCount(), metadata_,
		DDS_FLAGS_NONE, file_path.c_str());
	assert(SUCCEEDED(result));

	// 読み込んだテクスチャをSRGBとして扱う
	metadata_.format = MakeSRGB(metadata_.format);
}

wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string &m_string)
{
	// 文字数を計算
	int path_size = MultiByteToWideChar(CP_ACP, 0, m_string.c_str(), -1, nullptr, 0);

	wstring w_string;
	w_string.resize(path_size);

	// ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, m_string.c_str(), -1, &w_string[0], path_size);

	return w_string;
}

std::wstring TextureConverter::ReturnFileExtention(const std::wstring &path)
{
	size_t pos_1;

	// 区切り文字 '.' が出てくる一番最後の部分を検索
	pos_1 = path.rfind('.');

	// 検索がヒットしたら
	if (pos_1 != wstring::npos)
	{
		// 区切り文字の後ろをファイル拡張子として保存
		return path.substr(pos_1 + 1, path.size() - pos_1 - 1);
	}
	else
	{
		return L"";
	}
}
