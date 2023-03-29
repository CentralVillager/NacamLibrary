#pragma once

/*
	�~�T�C���Ɋւ���ݒ�\���̂Ȃǂ��܂Ƃ߂��w�b�_�ł��B
*/

/// <summary>
/// ���˕��@
/// </summary>
enum class MissileType
{
	Charge,
	Ultimate,
	Mono,
	ForEnemy
};

/// <summary>
/// �ǔ��̐��x
/// </summary>
enum class HomingAccuracy
{
	High,
	Middle,
	Low,
	NoHoming
};

/// <summary>
/// �~�T�C���̔��ːl
/// </summary>
enum class LaunchedBy
{
	Player,
	Enemy
};

/// <summary>
/// �^�[�Q�b�g�ƂȂ肤��I�u�W�F�N�g�̎��
/// </summary>
enum class TargetIs
{
	Player = -2
};

/// <summary>
/// �~�T�C�������p�����[�^
/// </summary>
struct MissileParam
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	MissileType type;			// �~�T�C���̔��˕��@

	float speed;				// ���葬�x

	XMFLOAT3 pos;				// �ʒu
	XMFLOAT3 vel;				// ���ݑ��x
	XMFLOAT3 acc;				// �����x

	XMFLOAT3 tgt_pos;			// �^�[�Q�b�g�̈ʒu
	int32_t tgt_id;				// �^�[�Q�b�g��ID

	float detection_range;		// �^�[�Q�b�g�T�m�͈�
	UINT init_straight_time;	// �ǔ����J�n����܂ł̎���
	bool use_homing_time;		// �ǔ����Ԃ�݂��邩
	UINT homing_time;			// �ǔ�����
	UINT life;					// ����
	bool is_validity;			// �~�T�C�����L����
	bool is_explode;			// ��������

	XMFLOAT3 trail_color;		// �O�Ղ̐F

	HomingAccuracy accuracy_type;	// �ǔ����x

	MissileParam() :
		type(),
		speed(),
		pos(),
		vel(),
		acc(),
		tgt_pos(),
		tgt_id(),
		detection_range(),
		init_straight_time(),
		use_homing_time(),
		homing_time(),
		life(),
		is_validity(),
		is_explode(),
		trail_color(),
		accuracy_type()
	{}
};
