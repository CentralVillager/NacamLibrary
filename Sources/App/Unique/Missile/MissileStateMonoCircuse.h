#pragma once
#include "IMissileLaunchState.h"
#include <DirectXMath.h>

class MissileStateMonoCircuse : public IMissileLaunchState
{
	bool FireMissile(const DirectX::XMFLOAT3 &launch_pos, const DirectX::XMFLOAT3 &dest_pos, MissileManager *ptr) override
	{
		// XMVECTOR�֕ϊ�
		DirectX::XMVECTOR launch = DirectX::XMLoadFloat3(&launch_pos);
		DirectX::XMVECTOR dest = DirectX::XMLoadFloat3(&dest_pos);

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
		MissileParam param{};
		param.pos = launch_pos;
		DirectX::XMStoreFloat3(&param.vel, direction_vec);
		// �����x�������_���ɐݒ�
		param.acc = NcmUtill::GenerateRandom(DirectX::XMFLOAT3(-1.5f, -1.5f, 0), DirectX::XMFLOAT3(1.5f, 1.5f, 0));
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
