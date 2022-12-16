#pragma once
#include "IMissileLaunchState.h"

class MissileStateUltimate : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// �~�T�C���̑����ː�
	static constexpr uint32_t ULT_LAUNCH_NUM_ = 20;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, const XMFLOAT3 &vec, MissileManager *ptr) override
	{
		// ���^�[�Q�b�g��
		static uint32_t all_tgt_num_ = 0;
		// �^�[�Q�b�g��̂ɂ��~�T�C���̐�
		static uint32_t contract_per_tgt_ = 0;
		// �Q�Ƃ���^�[�Q�b�g�̈ʒu�̋L�^
		static uint32_t tgt_location_ref_ = 0;
		// ���ˊԊu
		static const uint32_t DELAY = 3;
		// �J�E���g
		static uint32_t count = 1;
		// �����ː�
		static uint32_t launched = 0;
		// ULT�������̏���̔��˂�
		static bool is_first_launch = true;

		// ULT�������̏���̔��˂Ȃ�
		if (is_first_launch)
		{
			/* ���񔭎ˎ��̃f�[�^�����ɂ��� */
			// ���^�[�Q�b�g�����擾
			all_tgt_num_ = ptr->GetLockOnSys()->GetMaxTgtNum();
			// �^�[�Q�b�g��̂ɂ��~�T�C���̐����Z�o(�����˗\�萔 / ���^�[�Q�b�g��)
			contract_per_tgt_ = ULT_LAUNCH_NUM_ / all_tgt_num_;
			// ����t���O���~�낷
			is_first_launch = false;
			// �Q�Əꏊ�����Z�b�g
			tgt_location_ref_ = 0;
		}

		// �J�E���g�����炷
		count--;

		// �J�E���g��1�ȏ�Ȃ�
		if (count >= 1)
		{
			// �ȍ~�̏������X���[
			return false;
		}

		// �p�����[�^��ݒ�
		MissileParam param{};
		param.pos = launch_pos;
		param.vel = vec;
		// �����x�������_���ɐݒ�
		param.acc = NcmUtill::GenerateRandom(XMFLOAT3(-1.5f, -1.5f, 0), XMFLOAT3(1.5f, 1.5f, 0));
		// tgt_pos	 �͉��Őݒ�
		// tgt_index �͉��Őݒ�
		param.detection_range = 1000.0f;
		param.init_straight_time = 0;
		param.life = 300;

		// �ő働�b�N�I������10�ɐݒ�
		ptr->GetLockOnSys()->SetMaxTgtNum(10);

		// �^�[�Q�b�g�̎Q�Ƃ��擾
		auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), tgt_location_ref_);

		// �^�[�Q�b�g�f�[�^���擾
		param.tgt_pos = itr->pos;
		param.tgt_id = itr->id;

		// �~�T�C����ǉ�
		ptr->AddMissile(param);

		// ���ˉ񐔂����Z
		launched++;

		// �Q�Əꏊ��i�܂���
		tgt_location_ref_++;

		// �Q�Əꏊ�����^�[�Q�b�g�����z������
		if (tgt_location_ref_ > all_tgt_num_)
		{
			// �Q�Əꏊ���ŏ�����ɂ���
			tgt_location_ref_ = 0;
		}

		// �ő�^�[�Q�b�g�����f�t�H���g�ɖ߂�
		ptr->GetLockOnSys()->SetMaxTgtNum(LockOnSystem::DEFAULT_TGT_NUM_);

		// ���ˊԊu����������
		count = DELAY;

		// �����ː�������̐��ɒB������
		if (launched >= ULT_LAUNCH_NUM_)
		{
			// �p�����[�^�����Z�b�g
			launched = 0;
			count = 1;

			// �I����m�点��
			return true;
		}

		// ���s����m�点��
		return false;
	}
};
