#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "MissileDescs.h"
#include "MissileHoming.h"
#include "../../Lib/Math/Easing/NcmEasing.h"

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

	inline static Player *p_player_;

private:

	// �ǔ����x
	std::unique_ptr<MissileHoming> homing_sequence_;

	std::unique_ptr<Emitter> emitter_;		// �G�~�b�^�[
	std::unique_ptr<Emitter> explo_emi_;	// �����G�~�b�^�[
	MissileParam mi_param_;					// �p�����[�^

	ncm_ehandle for_lerp_handle_x_;			// ��ԗp�C�[�W���O�n���h��
	ncm_ehandle for_lerp_handle_y_;			// ��ԗp�C�[�W���O�n���h��
	ncm_ehandle for_lerp_handle_z_;			// ��ԗp�C�[�W���O�n���h��

	// �p�[�e�B�N����ԗp
	XMFLOAT3 before_particle_pos_;
	XMFLOAT3 current_particle_pos_;

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
	/// <param name="enemies"></param>emitter
	void UpdateTargetPos(EnemiesList &enemies);

	/// <summary>
	/// �ݒ肵���ʒu�ɒǔ�����
	/// </summary>
	void HomingTarget(HomingAccuracy accuracy);

	/// <summary>
	/// �G�~�b�^�[�̏I������������
	/// </summary>
	void PrepareTermEmitter();

private:

	/// <summary>
	/// �G�~�b�^�[�Ɋւ���X�V����
	/// </summary>
	void UpdateEmitter();

	/// <summary>
	/// �~�T�C���p�[�e�B�N���̕�ԏ���
	/// </summary>
	void InterpolateParticle();

	/// <summary>
	/// XMFLOAT3�^�̑S�Ẵ����o��0���̔���
	/// </summary>
	/// <param name="src">����f��</param>
	/// <returns>�S��0�Ȃ�true</returns>
	inline bool IsAllZero(XMFLOAT3 &src)
	{
		if (src.x == 0.0f && src.y == 0.0f && src.z == 0.0f)
		{
			return true;
		}

		return false;
	}
};

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3 &lhs, const float &rhs);
