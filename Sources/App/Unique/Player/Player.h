#pragma once
#include <memory>
#include "../Reticle/Reticle.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "../Ultimate/UltimateManager.h"
#include "../Missile/MissileLauncher.h"

class Camera;

/// <summary>
/// �v���C���[
/// </summary>
class Player : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

private:

	// ���G����
	static constexpr uint32_t INVINCIBLE_TIME_ = 80;

	// �ړ��\�͈�
	static constexpr XMFLOAT2 MOVABLE_RANGE_{ 20.0f, 15.0f };

	static constexpr float NORMAL_FOV_ = 60.0f;	// �ʏ펋��p
	static constexpr float ACCEL_FOV_ = 70.0f;	// ����������p

	static Camera *cam_ptr_;

private:

	// �`��f�[�^
	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	float normal_speed_;			// �ʏ펞���x

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
	ncm_ehandle ease_rot_right_;	// �E�X�|���̎p���J�ڗp
	ncm_ehandle ease_rot_left_;		// ���X�|���̎p���J�ڗp
	ncm_ehandle ease_reset_rot_;	// �p�����Z�b�g�p

	ncm_ehandle ease_move_left_;	// �E�ړ����̈ʒu�J�ڗp
	ncm_ehandle ease_move_right_;	// ���ړ����̈ʒu�J�ڗp
	ncm_ehandle ease_move_reset_;	// ���Z�b�g�p

	ncm_ehandle acc_speed_;			// ���������x
	ncm_ehandle dec_speed_;			// ���������x

	ncm_ehandle fov_acc_value_;		// ����������p�J��
	ncm_ehandle fov_dec_value_;		// ����������p�J��

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
	inline void SetCamPtr(Camera *p) { cam_ptr_ = p; }

	void Initialize() override;
	void Initialize(LockOnSystem *lockon_sys, UltimateManager *ult, const XMFLOAT3 &pos);
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// �~�T�C�����`���[�W����
	/// </summary>
	void ChargeMissile();

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	void TakeDamage();

	/// <summary>
	/// ���G���Ԃ��J�E���g����
	/// </summary>
	void CountInvincibleTime();

	/// <summary>
	/// ���E�������ւ̃R���g���[��
	/// </summary>
	/// <param name="speed">���x</param>
	void MoveXZ(float speed);

	/// <summary>
	/// ���E�c�����ւ̃R���g���[��
	/// </summary>
	/// <param name="speed">���x</param>
	void MoveXY(float speed);

	/// <summary>
	/// �񓪏���
	/// </summary>
	/// <param name="speed">���x</param>
	void RotationY(float speed);

	/// <summary>
	/// �����őO�ɐi�܂���
	/// </summary>
	/// <param name="speed">���x</param>
	/// <returns></returns>
	volatile void MoveForwardAuto(float speed);

	/// <summary>
	/// �O�ɐi�܂���
	/// </summary>
	/// <param name="speed">���x</param>
	void MoveZ(float speed);

	/// <summary>
	/// ����������
	/// </summary>
	void Accelerate();

	void RotPoseLeft(XMFLOAT3 &rot);
	void RotPoseRight(XMFLOAT3 &rot);
	void ResetRotPose(XMFLOAT3 &rot);
};
