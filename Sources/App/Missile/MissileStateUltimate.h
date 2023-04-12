#pragma once
#include <DirectXMath.h>
#include "IMissileLaunchState.h"
#include "../../Lib/Utility/NcmColor.h"

class MissileStateUltimate : public IMissileLaunchState
{
	// �~�T�C���̑����ː�
	static constexpr uint32_t ULT_LAUNCH_NUM_ = 30;

public:

	bool FireMissile(LaunchedBy launcher, const DirectX::XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		/* �v���C���[�݂̂��g�p����z��Ȃ̂�launcher�͎g�p���Ȃ� */

		// ���^�[�Q�b�g��
		static uint32_t all_tgt_num = 0;
		// �^�[�Q�b�g��̂ɂ��~�T�C���̐�
		static uint32_t contract_per_tgt = 0;
		// �Q�Ƃ���^�[�Q�b�g�̈ʒu�̋L�^
		static uint32_t tgt_location_ref = 0;
		// ���ˊԊu
		static const uint32_t DELAY = 2;
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
			all_tgt_num = ptr->GetLockOnSys()->GetMaxTgtNum();
			// �^�[�Q�b�g��̂ɂ��~�T�C���̐����Z�o(�����˗\�萔 / ���^�[�Q�b�g��)
			contract_per_tgt = ULT_LAUNCH_NUM_ / all_tgt_num;
			// ����t���O���~�낷
			is_first_launch = false;
			// �Q�Əꏊ�����Z�b�g
			tgt_location_ref = 0;
		}

		// �J�E���g�����炷
		count--;

		// �J�E���g��1�ȏ�Ȃ�
		if (count >= 1)
		{
			// �ȍ~�̏������X���[
			return false;
		}

		// �^�[�Q�b�g�̎Q�Ƃ��擾
		auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), tgt_location_ref);

		// �^�[�Q�b�g�f�[�^���擾
		MissileParam param{};

		if (launched % 2 == 0)
		{
			param.accuracy_type = HomingAccuracy::Linear;
			param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-0.2f, -0.2f, 0), DirectX::XMFLOAT3(0.2f, 0.2f, 0));
		}
		else
		{
			param.accuracy_type = HomingAccuracy::High;
			param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-1.5f, -1.5f, 0), DirectX::XMFLOAT3(1.5f, 1.5f, 0));
		}
		param.tgt_pos = itr->pos;
		param.tgt_id = itr->id;

		// XMVECTOR�֕ϊ�
		DirectX::XMVECTOR launch = DirectX::XMLoadFloat3(&launch_pos);
		DirectX::XMVECTOR dest = DirectX::XMLoadFloat3(&param.tgt_pos);

		// �����x�N�g�����Z�o
		DirectX::XMVECTOR direction_vec =
		{
			(dest.m128_f32[0] - launch.m128_f32[0]),
			(dest.m128_f32[1] - launch.m128_f32[1]),
			(dest.m128_f32[2] - launch.m128_f32[2])
		};
		// ���K��
		direction_vec = DirectX::XMVector3Normalize(direction_vec);

		// �p�����[�^��ݒ�
		param.speed = 10.0f;
		param.pos = launch_pos;
		DirectX::XMStoreFloat3(&param.vel, direction_vec);
		// tgt_pos	 �͉��Őݒ�
		// tgt_index �͉��Őݒ�
		param.use_homing_time = false;
		param.detection_range = 1000.0f;
		param.init_straight_time = 0;
		param.life = NcmUtill::GenerateRandom(200, 300);
		param.trail_color = NcmColor::TRAIL_WHITE;

		// �ő働�b�N�I������10�ɐݒ�
		ptr->GetLockOnSys()->SetMaxTgtNum(10);

		// �~�T�C����ǉ�
		ptr->AddMissile(param);

		// ���ˉ񐔂����Z
		launched++;

		// �Q�Əꏊ��i�܂���
		tgt_location_ref++;

		// �Q�Əꏊ�����^�[�Q�b�g�����z������
		if (tgt_location_ref > all_tgt_num)
		{
			// �Q�Əꏊ���ŏ�����ɂ���
			tgt_location_ref = 0;
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
