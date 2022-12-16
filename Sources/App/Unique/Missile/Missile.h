#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"

/// <summary>
/// �~�T�C�������p�����[�^
/// </summary>
struct MissileParam
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
	bool is_explode;			// ��������

	MissileParam() :
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

/// <summary>
/// �~�T�C��
/// </summary>
class Missile : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:

	/// <summary>
	/// �^�[�Q�b�g��������
	/// </summary>
	struct SearchResult
	{
		bool is_succes;
		XMFLOAT3 pos;

		SearchResult() :
			is_succes(),
			pos()
		{}
	};

private:

	// �����p�[�e�B�N���̗��̐��ڈ�
	static constexpr uint32_t EXPLO_PARTICLE_NUM_ = 100;

private:

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

private:

	std::unique_ptr<Emitter> emitter_;	// �G�~�b�^�[
	std::unique_ptr<Emitter> explo_emi_;// �����G�~�b�^�[
	MissileParam mi_param_;				// �p�����[�^

public:

	Missile();
	~Missile();

public:

	static void LoadResources();

	void Initialize(const MissileParam &args);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	const bool &GetIsValidity() { return mi_param_.is_validity; }

	void SetMissileLife(const int life) { mi_param_.life = life; }
	void SetTgtPos(const XMFLOAT3 &pos) { mi_param_.tgt_pos = pos; }

	/// <summary>
	/// ���S�t���O���܂߂ă~�T�C����L����
	/// </summary>
	void Activate();

	/// <summary>
	/// ���S�t���O�͂��̂܂܂Ƀ~�T�C���𖳌���
	/// </summary>
	void InvalidateMissile();

	/// <summary>
	/// ���S���̔���
	/// </summary>
	void ExplosionOnDeath();

	/// <summary>
	/// z�������֐i��
	/// </summary>
	void MoveZ(float speed);

	/// <summary>
	/// �^�[�Q�b�g�̈ʒu���Z�o����
	/// </summary>
	/// <param name="enemies"></param>
	/// <returns></returns>
	SearchResult CalcEnemyPos(EnemiesList &enemies);

	/// <summary>
	/// �G��ǔ�����
	/// </summary>
	/// <param name="enemies">�G�̃R���e�i</param>
	void HomingEnemy(EnemiesList &enemies);

	void TestHomingTarget(EnemiesList &enemies);

	/// <summary>
	/// �w�肵���ʒu�ɒǔ�����
	/// </summary>
	/// <param name="target"></param>
	void HomingTarget(XMFLOAT3 &target);

	/// <summary>
	/// �G�~�b�^�[�̏I������������
	/// </summary>
	void PrepareTermEmitter();

private:

	void UpdateEmitter();
};
