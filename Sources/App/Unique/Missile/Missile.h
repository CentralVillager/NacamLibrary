#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "MissileDescs.h"

class Player;

/// <summary>
/// �~�T�C��
/// </summary>
class Missile : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

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

	inline static Player *p_player_;

public:

	Missile();
	~Missile();

public:

	static void SetPtr(Player *p)
	{
		p_player_ = p;
	}
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
	MissileParam &GetMissileParam() { return mi_param_; }

	void SetMissileLife(const int life) { mi_param_.life = life; }
	void SetTgtPos(const XMFLOAT3 &pos) { mi_param_.tgt_pos = pos; }

public:

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
	/// �G�̈ʒu���Z�o����
	/// </summary>
	/// <param name="enemies"></param>
	/// <returns></returns>
	void UpdateTargetPos(EnemiesList &enemies);

	/// <summary>
	/// �G��ǔ�����
	/// </summary>
	/// <param name="enemies">�G�̃R���e�i</param>
	void HomingEnemy(EnemiesList &enemies);

	void TestHomingTarget(EnemiesList &enemies);

	/// <summary>
	/// �ݒ肵���ʒu�ɒǔ�����
	/// </summary>
	void HomingTarget();

	/// <summary>
	/// �G�~�b�^�[�̏I������������
	/// </summary>
	void PrepareTermEmitter();

private:

	/// <summary>
	/// �G�~�b�^�[�Ɋւ���X�V����
	/// </summary>
	void UpdateEmitter();
};
