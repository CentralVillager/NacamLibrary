#pragma once
#include <string>
#include <DirectXTex.h>

class TextureConverter
{
private:

	DirectX::TexMetadata metadata_;			// 画像の情報
	DirectX::ScratchImage scratch_image_;	// 画像イメージのコンテナ

	std::wstring directory_path_;	// ディレクトリパス
	std::wstring file_name_;		// ファイル名
	std::wstring file_ext_;			// ファイル拡張子

public:

	/// <summary>
	/// WICからDDS形式に変換する
	/// </summary>
	/// <param name="path"></param>
	void ConvertTextureWICToDDS(const std::string &path);

private:

	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="path">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string &path);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="path">ファイルパス</param>
	void SeparateFilePath(const std::wstring &path);

	/// <summary>
	/// DDSテクスチャとしてファイル書き出し
	/// </summary>
	void SaveDDSTextureToFile();

public:

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="m_string"></param>
	/// <returns></returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string &m_string);

	/// <summary>
	/// ファイル拡張子を返す
	/// </summary>
	/// <param name="m_string"></param>
	/// <returns></returns>
	static std::wstring ReturnFileExtention(const std::wstring &path);
};
