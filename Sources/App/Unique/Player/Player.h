#pragma once
#include <memory>
#include "../Reticle/Reticle.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "../Ultimate/UltimateManager.h"
#include "../Missile/MissileLauncher.h"

/// <summary>
/// �v���C���[
/// </summary>
class Player : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	static constexpr uint32_t invincible_time_ = 80;

private:

	// �`��f�[�^
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	bool is_invincible_;			// ���G��Ԃ��ǂ���
	bool taking_damage_trigger_;	// �_���[�W���󂯂��u�Ԃ�
	bool is_triggering_ult_;		// �E���g��������u�Ԃ�
	uint32_t hp_;					// HP

	// ���N���X���
	LockOnSystem *p_lockon_sys_;
	UltimateManager *p_ult_;

	// �~�T�C�����ˊ֘A
	std::unique_ptr<MissileLauncher> mi_launcher_;
	int32_t charge_time_;			// ���˂ɕK�v�ȃ`���[�W����
	int32_t count_;					// �`���[�W�J�E���g�p

	// �C�[�W���O�p�ϐ�
	int32_t ease_rot_right_;		// �E�X�|���̎p���J�ڗp
	int32_t ease_rot_left_;			// ���X�|���̎p���J�ڗp
	int32_t ease_reset_rot_;		// �p�����Z�b�g�p

	bool is_already_;
	bool is_released;

	float rot_angle_;				// �X�|����p�x

public:

	Player();
	~Player();

public:

	static void LoadResources();

	inline const bool &GetIsInvincible() { return is_invincible_; }
	inline const bool &GetIsTriggeringUlt() { return is_triggering_ult_; }
	inline const uint32_t &GetHp() { return hp_; }
	inline const int32_t &GetChargeCount() { return count_; }
	inline const int32_t &GetMaxChargeTime() { return charge_time_; }

	void Initialize() override;
	void Initialize(LockOnSystem *lockon_sys, UltimateManager *ult, XMFLOAT3 pos);
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	void ChargeMissile();
	void TakeDamage();
	void CountInvincibleTime();
	void Move(float speed);
	void MoveXY(float speed);
	void MoveXZ(float speed);
	void RotationY(float speed);
	void MoveForwardAuto();

	void RotPoseLeft(XMFLOAT3 &rot);
	void RotPoseRight(XMFLOAT3 &rot);
	void ResetRotPose(XMFLOAT3 &rot);
};
