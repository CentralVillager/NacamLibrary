#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateHighAccuracy : public IMissileHomingState
{
	void HomingTarget(XMFLOAT3 dest_pos) override
	{
		using namespace DirectX;
		using namespace NcmUtill;
		using namespace NcmMath;

		MissileParam *mi_param = &missile_->GetMissileParam();

		// �~�T�C���̌��݈ʒu���擾
		mi_param->pos = missile_->GetPos();

		// XMVECTOR�ɕϊ�
		XMVECTOR mi_vec = XMLoadFloat3(&mi_param->pos);
		XMVECTOR tgt_vec = XMLoadFloat3(&mi_param->tgt_pos);

		// �ӂ��̍��W�����ԃx�N�g�����v�Z
		XMVECTOR vec =
		{
			(tgt_vec.m128_f32[0] - mi_vec.m128_f32[0]),
			(tgt_vec.m128_f32[1] - mi_vec.m128_f32[1]),
			(tgt_vec.m128_f32[2] - mi_vec.m128_f32[2])
		};

		// �������擾
		XMVECTOR len = XMVector3Length(vec);

		// �ǔ��͈͊O�Ȃ�
		if (len.m128_f32[0] >= mi_param->detection_range)
		{
			// ���i��������
			missile_->MoveZ(missile_->GetSpeed());

			// ���̌�̒ǔ��������X�L�b�v
			return;
		}

		// ���K��
		XMVECTOR norm_vec = XMVector3Normalize(vec);
		XMVECTOR vel_vec = XMLoadFloat3(&mi_param->vel);
		XMVECTOR mi_norm_vec = XMVector3Normalize(vel_vec);

		// ���x���Z�b�g
		XMStoreFloat3(&mi_param->vel, norm_vec);

		// �����x�����Z
		mi_param->vel.x += mi_param->acc.x;
		mi_param->vel.y += mi_param->acc.y;
		mi_param->vel.z += mi_param->acc.z;

		if (IsPlus(mi_param->acc.x)) { mi_param->acc.x -= 0.01f; }
		else if (IsMinus(mi_param->acc.x)) { mi_param->acc.x += 0.01f; }

		if (IsPlus(mi_param->acc.y)) { mi_param->acc.y -= 0.01f; }
		else if (IsMinus(mi_param->acc.y)) { mi_param->acc.y += 0.01f; }

		if (IsPlus(mi_param->acc.z)) { mi_param->acc.z -= 0.01f; }
		else if (IsMinus(mi_param->acc.z)) { mi_param->acc.z += 0.01f; }

		// ���x�����Z
		mi_param->vel.x *= missile_->GetSpeed();
		mi_param->vel.y *= missile_->GetSpeed();
		mi_param->vel.z *= missile_->GetSpeed();

		// �ʒu���X�V
		mi_param->pos.x += mi_param->vel.x;
		mi_param->pos.y += mi_param->vel.y;
		mi_param->pos.z += mi_param->vel.z;

		// �ʒu�𔽉f
		missile_->SetPos(mi_param->pos);

		missile_->SetRot(LookAt(mi_param->vel));
	}
};
