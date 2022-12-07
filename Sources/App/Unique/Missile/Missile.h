#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"

/// <summary>
/// �~�T�C�������p�����[�^
/// </summary>
struct MissileArgs
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 pos;				// �ʒu
	XMFLOAT3 vel;				// ���x
	XMFLOAT3 acc;				// �����x

	XMFLOAT3 tgt_pos;			// �^�[�Q�b�g�̈ʒu
	int tgt_id;					// �^�[�Q�b�g��ID

	float detection_range;		// �^�[�Q�b�g�T�m�͈�
	UINT init_straight_time;	// �ǔ����J�n����܂ł̎���
	UINT life;					// ����
	bool is_validity;			// �~�T�C�����L����

	MissileArgs() :
		pos(XMFLOAT3(0, 0, 0)),
		vel(XMFLOAT3(0, 0, 0)),
		acc(XMFLOAT3(0, 0, 0)),
		tgt_pos(XMFLOAT3(0, 0, 0)),
		tgt_id(0),
		detection_range(0),
		init_straight_time(0),
		life(0),
		is_validity(false)
	{}
};

class Missile : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	std::unique_ptr<Emitter> emitter_;	// �G�~�b�^�[
	MissileArgs mi_args_;				// �p�����[�^

public:

	Missile();
	~Missile();

public:

	static void LoadResources();

	void Initialize(const MissileArgs &args);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	const bool &GetIsValidity() { return mi_args_.is_validity; }

	void SetMissileLife(const int life) { mi_args_.life = life; }
	void SetTgtPos(const XMFLOAT3 &pos) { mi_args_.tgt_pos = pos; }

	/// <summary>
	/// ���S�t���O���܂߂ă~�T�C����L����
	/// </summary>
	void Activate();

	/// <summary>
	/// ���S�t���O�͂��̂܂܂Ƀ~�T�C���𖳌���
	/// </summary>
	void InvalidateMissile() { mi_args_.is_validity = false; }

	/// <summary>
	/// z�������֐i��
	/// </summary>
	void MoveZ(float speed);

	/// <summary>
	/// �^�[�Q�b�g��ǔ�����
	/// </summary>
	void HomingTarget(EnemiesList &enemies);

	void TestHomingTarget(EnemiesList &enemies);

	/// <summary>
	/// �G�~�b�^�[�̏I������������
	/// </summary>
	void PrepareTermEmitter();

private:

	void UpdateEmitter();
};
