#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <memory>

/// <summary>
/// ファイルからレベル情報を読み込みます。
/// </summary>
class LevelDataManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	/// <summary>
	/// オブジェクト1つが持つ情報
	/// </summary>
	struct ObjectData
	{
		std::string file_name;	// ファイル名
		XMFLOAT3 pos;			// 位置
		XMFLOAT3 rot;			// 回転
		XMFLOAT3 scale;			// スケーリング
	};

	/// <summary>
	/// レベル全体の情報
	/// </summary>
	struct LevelData
	{
		std::vector<ObjectData> object_data;
	};

private:

	/// <summary>
	/// レベル情報を格納
	/// </summary>
	std::unique_ptr<LevelData> level_datas_;

public:

	LevelDataManager();
	~LevelDataManager();

public:

	const std::unique_ptr<LevelData> &GetLevelDatas() { return level_datas_; }

public:

	/// <summary>
	/// jsonファイルからレベル情報を読み込む
	/// </summary>
	/// <param name="path"></param>
	void LoadLevelByJson(std::string path);
};
