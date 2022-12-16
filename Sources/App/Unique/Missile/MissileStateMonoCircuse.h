#pragma once
#include "IMissileLaunchState.h"

class MissileStateMonoCircuse : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, const XMFLOAT3 &vec, MissileManager *ptr) override
	{
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

		// �^�[�Q�b�g�̎Q�Ƃ��擾
		auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), 0);

		// �^�[�Q�b�g�f�[�^���擾
		param.tgt_pos = itr->pos;
		param.tgt_id = itr->id;

		// �~�T�C����ǉ�
		ptr->AddMissile(param);

		return false;
	}
};
