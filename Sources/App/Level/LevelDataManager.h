#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <memory>

/// <summary>
/// �t�@�C�����烌�x������ǂݍ��݂܂��B
/// </summary>
class LevelDataManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	/// <summary>
	/// �I�u�W�F�N�g1�������
	/// </summary>
	struct ObjectData
	{
		std::string file_name;	// �t�@�C����
		XMFLOAT3 pos;			// �ʒu
		XMFLOAT3 rot;			// ��]
		XMFLOAT3 scale;			// �X�P�[�����O
	};

	/// <summary>
	/// ���x���S�̂̏��
	/// </summary>
	struct LevelData
	{
		std::vector<ObjectData> object_data;
	};

private:

	/// <summary>
	/// ���x�������i�[
	/// </summary>
	std::unique_ptr<LevelData> level_datas_;

public:

	LevelDataManager();
	~LevelDataManager();

public:

	const std::unique_ptr<LevelData> &GetLevelDatas() { return level_datas_; }

public:

	/// <summary>
	/// json�t�@�C�����烌�x������ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	void LoadLevelByJson(std::string path);
};
