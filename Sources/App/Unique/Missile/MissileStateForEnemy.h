#pragma once
#include "IMissileLaunchState.h"
#include "../../Utility/NcmColor.h"

class MissileStateForEnemy : public IMissileLaunchState
{
	bool FireMissile(LaunchedBy launcher, const DirectX::XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		MissileParam param{};
		param.type = MissileType::ForEnemy;

		switch (launcher)
		{
		case LaunchedBy::Enemy:

			// ID���v���C���[�̂��̂Ƃ���
			param.tgt_id = (int32_t)(TargetIs::Player);
			param.tgt_pos = DirectX::XMFLOAT3();
			param.life = 200;
			break;

		default:
			break;
		}

		// �p�����[�^��ݒ�
		param.speed = NcmUtill::GenerateRandom(0.5f, 3.0f);
		param.pos = launch_pos;
		// �����x�������_���ɐݒ�
		param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-1.5f, -1.5f, 0), DirectX::XMFLOAT3(1.5f, 1.5f, 0));
		param.detection_range = 1000.0f;
		param.use_homing_time = true;
		param.homing_time = NcmUtill::GenerateRandom(50, 100);
		param.init_straight_time = 0;
		param.trail_color = NcmColor::TRAIL_RED;

		// �ˏo�ʒu�ƖڕW�ʒu��XMVECTOR�֕ϊ�
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

		// �������x��ݒ�
		DirectX::XMStoreFloat3(&param.vel, direction_vec);

		// �~�T�C����ǉ�
		ptr->AddMissile(param);

		return false;
	}
};
