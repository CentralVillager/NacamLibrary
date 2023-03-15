#pragma once
#include "IMissileLaunchState.h"
#include "../../Utility/NcmColor.h"

class MissileStateCharge : public IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	bool FireMissile(LaunchedBy launcher, const DirectX::XMFLOAT3 &launch_pos, MissileManager *ptr) override
	{
		MissileParam param{};

		switch (launcher)
		{
		case LaunchedBy::Enemy:

			// ID���v���C���[�̂��̂Ƃ���
			param.tgt_id = (int32_t)(TargetIs::Player);
			// tgt_pos �͌�ɏ�������
			break;

		case LaunchedBy::Player:
		{
			// �^�[�Q�b�g�̎Q�Ƃ��擾
			auto itr = NcmUtill::MoveIterator(ptr->GetLockOnSys()->GetTgtList().begin(), 0);

			if (ptr->GetLockOnSys()->GetTgtList().empty())
			{
				return false;
			}

			// �擾����ID���G�ő吔���傫��������
			if (ptr->GetLockOnSys()->GetTgtList().size() < itr->id)
			{
				// ���̓G�͑��݂��Ȃ�
				param.tgt_id = (int32_t)(NacamError::NonDetected);
				param.tgt_pos = DirectX::XMFLOAT3();
			}
			else
			{
				// �^�[�Q�b�g�f�[�^���擾
				param.tgt_pos = itr->pos;
				param.tgt_id = itr->id;
			}
		}
		break;

		default:
			break;
		}

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

		param.speed = 4.0f;
		param.pos = launch_pos;
		DirectX::XMStoreFloat3(&param.vel, direction_vec);
		// tgt_pos �͉��Őݒ�
		// tgt_id �͉��Őݒ�
		param.detection_range = 1000.0f;
		param.use_homing_time = false;
		param.init_straight_time = 0;
		param.life = 300;
		param.trail_color = NcmColor::TRAIL_WHITE;

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
