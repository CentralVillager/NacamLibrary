#pragma once
#include "IMissileHomingState.h"
#include "Missile.h"

class Missile;

class MissileHomingStateHighAccuracy : public IMissileHomingState
{
	void HomingTarget(Missile &missile) override
	{
		using namespace DirectX;
		using namespace NcmUtill;
		using namespace NcmMath;

		MissileParam *mi_param = &missile.GetMissileParam();

		// �~�T�C���̌��݈ʒu���擾
		mi_param->pos = missile.GetPos();

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
			missile.MoveZ(missile.GetSpeed());

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
		mi_param->vel.x *= missile.GetSpeed();
		mi_param->vel.y *= missile.GetSpeed();
		mi_param->vel.z *= missile.GetSpeed();

		// �ʒu���X�V
		mi_param->pos.x += mi_param->vel.x;
		mi_param->pos.y += mi_param->vel.y;
		mi_param->pos.z += mi_param->vel.z;

		// �ʒu�𔽉f
		missile.SetPos(mi_param->pos);

		missile.SetRot(LookAt(mi_param->vel));

		if (mi_param->type == MissileType::ForEnemy)
		{
			if (len.m128_f32[0] <= GenerateRandom(5.0f, 50.0f))
			{
				// �G�~�b�^�[�̏I������
				missile.PrepareTermEmitter();
				// �~�T�C���̎����������I��0��
				missile.SetMissileLife(0);
				// �~�T�C���𖳌���(���S�t���O�͌��ĂȂ�)
				missile.InvalidateMissile();
			}
		}
	}
};
