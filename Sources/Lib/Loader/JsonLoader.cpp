#include "JsonLoader.h"
#include <cassert>
#include "../../App/Level/LevelDataManager.h"

using json = nlohmann::json;
using namespace std;

void NcmJsonLoader::OpenJsonFile(std::string path)
{
	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(path);
	assert(!file.fail());

	// json文字列から解凍したデータ
	json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelDataManager::LevelData *level_data = new LevelDataManager::LevelData();

	// "object"の全オブジェクトを走査
	for (json &object : deserialized["object"])
	{
		assert(object.contains("type"));

		// 種別を取得
		string type = object["type"].get<string>();

		// MESH
		if (type.compare("MESH") == 0)
		{
			// 要素追加
			level_data->object_data.emplace_back(LevelDataManager::ObjectData{});

			// 参照を得る
			LevelDataManager::ObjectData &obj_data = level_data->object_data.back();

			// ファイル名
			if (object.contains("file_name"))
			{
				obj_data.file_name = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			json &transform = object["transform"];

			// 平行移動
			obj_data.pos.x = (float)(transform["translation"][1]);
			obj_data.pos.y = (float)(transform["translation"][2]);
			obj_data.pos.z = -(float)(transform["translation"][0]);
			// 回転
			obj_data.rot.x = -(float)(transform["rotation"][1]);
			obj_data.rot.y = -(float)(transform["rotation"][2]);
			obj_data.rot.z = (float)(transform["rotation"][0]);
			// スケーリング
			obj_data.scale.x = (float)(transform["scaling"][1]);
			obj_data.scale.y = (float)(transform["scaling"][2]);
			obj_data.scale.z = (float)(transform["scaling"][0]);

			// TODO : オブジェクト走査を再帰関数にまとめ、再帰呼び出しで枝を走査する
			if (object.contains("children"))
			{

			}
		}
	}

	delete level_data;
	file.close();
}
