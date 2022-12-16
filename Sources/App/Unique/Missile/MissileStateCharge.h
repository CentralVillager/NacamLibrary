#pragma once
#include "IMissileLaunchState.h"

class MissileStateCharge : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, const XMFLOAT3 &vec, MissileManager *ptr) override
	{
		MissileParam param{};
		param.pos = launch_pos;
		param.vel = vec;
		// acc ��MissileManager�Őݒ�
		// tgt_pos ��MissileManager�Őݒ�
		// tgt_index ��MissileManager�Őݒ�
		param.detection_range = 1000.0f;
		param.init_straight_time = 0;
		param.life = 300;

		// ���b�N������܂�
		for (UINT i = 0; i < ptr->GetLockOnSys()->GetCurrentTgtNum(); i++)
		{
			auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), i);

			// �^�[�Q�b�g�f�[�^���擾
			param.tgt_pos = itr->pos;
			param.tgt_id = itr->id;

			// �~�T�C����ǉ�
			ptr->AddMissile(param);
		}

		return false;	// ���ʉ��̂��߂̂�ނȂ����^�[��
	}
};
