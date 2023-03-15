#pragma once
#include "IMissileLaunchState.h"
#include <DirectXMath.h>
#include "../../../Lib/NacamError/NacamError.h"
#include "../../Utility/NcmColor.h"

class MissileStateMonoCircuse : public IMissileLaunchState
{
	bool FireMissile(LaunchedBy launcher, const DirectX::XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		MissileParam param{};
		param.type = MissileType::Mono;

		switch (launcher)
		{
		case LaunchedBy::Enemy:

			// ID���v���C���[�̂��̂Ƃ���
			param.tgt_id = (int32_t)(TargetIs::Player);
			param.tgt_pos = DirectX::XMFLOAT3();
			break;

		case LaunchedBy::Player:
		{
			// �^�[�Q�b�g�̎Q�Ƃ��擾
			auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), 0);

			// �^�[�Q�b�g�f�[�^���擾
			param.tgt_id = itr->id;
			param.tgt_pos = itr->pos;
		}
		break;

		default:
			break;
		}

		// �p�����[�^��ݒ�
		param.speed = 10.0f;
		param.pos = launch_pos;
		// �����x�������_���ɐݒ�
		param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-1.5f, -1.5f, 0), DirectX::XMFLOAT3(1.5f, 1.5f, 0));
		param.detection_range = 1000.0f;
		param.use_homing_time = false;
		param.init_straight_time = 0;
		param.life = NcmUtill::GenerateRandom(100, 300);
		param.trail_color = NcmColor::TRAIL_WHITE;
		//param.life = 300;

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
