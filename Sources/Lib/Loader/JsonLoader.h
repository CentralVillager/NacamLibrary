#pragma once
#include <string>
#include <fstream>
#undef snprintf
#include <json.hpp>
#include <vector>
#include <DirectXMath.h>

namespace NcmJsonLoader
{
	/// <summary>
	/// jsonファイルを読み込みます。
	/// </summary>
	/// <param name="path"></param>
	static void OpenJsonFile(std::string path);
}
