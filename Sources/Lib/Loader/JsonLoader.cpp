#include "JsonLoader.h"
#include <cassert>
#include "../../App/Level/LevelDataManager.h"

using json = nlohmann::json;
using namespace std;

void NcmJsonLoader::OpenJsonFile(std::string path)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(path);
	assert(!file.fail());

	// json�����񂩂�𓀂����f�[�^
	json deserialized;

	// ��
	file >> deserialized;

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	// ���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelDataManager::LevelData *level_data = new LevelDataManager::LevelData();

	// "object"�̑S�I�u�W�F�N�g�𑖍�
	for (json &object : deserialized["object"])
	{
		assert(object.contains("type"));

		// ��ʂ��擾
		string type = object["type"].get<string>();

		// MESH
		if (type.compare("MESH") == 0)
		{
			// �v�f�ǉ�
			level_data->object_data.emplace_back(LevelDataManager::ObjectData{});

			// �Q�Ƃ𓾂�
			LevelDataManager::ObjectData &obj_data = level_data->object_data.back();

			// �t�@�C����
			if (object.contains("file_name"))
			{
				obj_data.file_name = object["file_name"];
			}

			// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
			json &transform = object["transform"];

			// ���s�ړ�
			obj_data.pos.x = (float)(transform["translation"][1]);
			obj_data.pos.y = (float)(transform["translation"][2]);
			obj_data.pos.z = -(float)(transform["translation"][0]);
			// ��]
			obj_data.rot.x = -(float)(transform["rotation"][1]);
			obj_data.rot.y = -(float)(transform["rotation"][2]);
			obj_data.rot.z = (float)(transform["rotation"][0]);
			// �X�P�[�����O
			obj_data.scale.x = (float)(transform["scaling"][1]);
			obj_data.scale.y = (float)(transform["scaling"][2]);
			obj_data.scale.z = (float)(transform["scaling"][0]);

			// TODO : �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�Ăяo���Ŏ}�𑖍�����
			if (object.contains("children"))
			{

			}
		}
	}

	delete level_data;
	file.close();
}
