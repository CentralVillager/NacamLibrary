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
	Low
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

	XMFLOAT3 pos;				// �ʒu
	XMFLOAT3 vel;				// ���x
	XMFLOAT3 acc;				// �����x

	XMFLOAT3 tgt_pos;			// �^�[�Q�b�g�̈ʒu
	int32_t tgt_id;				// �^�[�Q�b�g��ID

	float detection_range;		// �^�[�Q�b�g�T�m�͈�
	UINT init_straight_time;	// �ǔ����J�n����܂ł̎���
	UINT life;					// ����
	bool is_validity;			// �~�T�C�����L����
	bool is_explode;			// ��������

	MissileParam() :
		type(),
		pos(),
		vel(),
		acc(),
		tgt_pos(),
		tgt_id(),
		detection_range(),
		init_straight_time(),
		life(),
		is_validity(),
		is_explode()
	{}
};
