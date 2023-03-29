#pragma once
#include <memory>
#include "../Abs/AbsUniqueObj.h"

class Player;
class MissileLauncher;

/// <summary>
/// �G
/// </summary>
class Enemy : public AbsUniqueObj
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// �~�T�C�����ˊԊu
	static constexpr uint32_t LAUNCH_MISSILE_INTERVAL_ = 300;

private:

	// ���f���f�[�^
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	// ID�t�^�p�J�E���^�[
	static int id_counter_;

	static Player *player_;

private:

	// �~�T�C��
	std::shared_ptr<MissileLauncher> mi_launcher_;

	// �̔��ʗpID
	uint32_t id_;

	// ���]�Ԋu
	int count_;

	// �~�^���p�A���O��
	float circular_angle_;

	// ���ˊԊu�ۑ��p
	uint32_t shot_interval_;

	// �~�T�C�����ˊԊu�ۑ��p
	uint32_t missile_launch_count_;

	int32_t cycle = 100;
	float length = 100.0f;

public:

	Enemy();
	~Enemy();

public:

	static void LoadResources();
	static void ImportPtr(Player *player);

public:

	const uint32_t &GetID() { return id_; }

public:

	void Initialize(const XMFLOAT3 &pos);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// y����]���s��
	/// </summary>
	void RotY();

	/// <summary>
	/// ���������Ɉړ�
	/// </summary>
	void MoveHorizontally(const float speed, const float range);

	/// <summary>
	/// z�������ֈړ�
	/// </summary>
	/// <param name="speed"></param>
	void MoveZ(const float speed);

	/// <summary>
	/// �~�^�����s��
	/// </summary>
	void MoveCircular();

	/// <summary>
	/// �~�T�C����P�����˂���
	/// </summary>
	void FireMissile();

	/// <summary>
	/// �~�T�C�����ˊ֘A�̓���
	/// </summary>
	void LaunchMissileSequence();

private:

	/// <summary>
	/// �~�T�C���𔭎˂ł��邩
	/// </summary>
	/// <returns></returns>
	bool CheckCanFire();
};