#pragma once
#include "IMissileLaunchState.h"

class MissileStateMulti : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(const XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		MissileArgs l_args{};
		l_args.pos = launch_pos;
		l_args.vel = XMFLOAT3(0, 0, 1.0f);
		//l_args.acc = XMFLOAT3(0, -1.0f, 0);
		// acc ��MissileManager�Őݒ�
		// tgt_pos ��MissileManager�Őݒ�
		// tgt_index ��MissileManager�Őݒ�
		l_args.detection_range = 1000.0f;
		l_args.init_straight_time = 0;
		l_args.life = 200;

		ptr->FireMultiMissile(l_args, 4);

		return false;	// ���ʉ��̂��߂̂�ނȂ����^�[��
	}
};
