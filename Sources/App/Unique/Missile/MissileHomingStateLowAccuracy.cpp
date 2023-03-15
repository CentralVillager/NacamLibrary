#include "MissileHomingStateLowAccuracy.h"
#include "Missile.h"
#include "MissileDescs.h"

using namespace DirectX;
using namespace NcmUtill;
using namespace NcmMath;

void MissileHomingStateLowAccuracy::HomingTarget(Missile &missile)
{
	MissileParam *mi_param = &missile.GetMissileParam();
	mi_param->accuracy_type = HomingAccuracy::Low;

	// 追尾時間が設定されているなら
	if (mi_param->use_homing_time)
	{
		// 追尾時間を減らす
		mi_param->homing_time--;

		// 追尾時間が0以下になったら
		if (IsZeroOrLess(mi_param->homing_time))
		{
			// 最終追尾時の速度を使用し、位置を更新
			mi_param->pos.x += mi_param->vel.x;
			mi_param->pos.y += mi_param->vel.y;
			mi_param->pos.z += mi_param->vel.z;

			// 位置を反映
			missile.SetPos(mi_param->pos);

			missile.SetRot(LookAt(mi_param->vel));

			return;
		}
	}

	// ミサイルの現在位置を取得
	mi_param->pos = missile.GetPos();

	// XMVECTORに変換
	XMVECTOR mi_vec = XMLoadFloat3(&mi_param->pos);
	XMVECTOR tgt_vec = XMLoadFloat3(&mi_param->tgt_pos);

	// ふたつの座標を結ぶベクトルを計算
	XMVECTOR vec =
	{
		(tgt_vec.m128_f32[0] - mi_vec.m128_f32[0]),
		(tgt_vec.m128_f32[1] - mi_vec.m128_f32[1]),
		(tgt_vec.m128_f32[2] - mi_vec.m128_f32[2])
	};

	vec.m128_f32[0] *= 0.5f;
	vec.m128_f32[1] *= 0.5f;
	vec.m128_f32[2] *= 0.5f;

	// 長さを取得
	XMVECTOR len = XMVector3Length(vec);

	// 追尾範囲外なら
	if (len.m128_f32[0] >= mi_param->detection_range)
	{
		// 直進だけして
		missile.MoveZ(missile.GetSpeed());

		// その後の追尾処理をスキップ
		return;
	}

	// 正規化
	XMVECTOR norm_vec = XMVector3Normalize(vec);
	XMVECTOR vel_vec = XMLoadFloat3(&mi_param->vel);
	XMVECTOR mi_norm_vec = XMVector3Normalize(vel_vec);

	// 速度をセット
	XMStoreFloat3(&mi_param->vel, norm_vec);

	// 加速度を加算
	mi_param->vel.x += mi_param->acc.x;
	mi_param->vel.y += mi_param->acc.y;
	mi_param->vel.z += mi_param->acc.z;

	if (IsPlus(mi_param->acc.x)) { mi_param->acc.x -= 0.01f; }
	else if (IsMinus(mi_param->acc.x)) { mi_param->acc.x += 0.01f; }

	if (IsPlus(mi_param->acc.y)) { mi_param->acc.y -= 0.01f; }
	else if (IsMinus(mi_param->acc.y)) { mi_param->acc.y += 0.01f; }

	if (IsPlus(mi_param->acc.z)) { mi_param->acc.z -= 0.01f; }
	else if (IsMinus(mi_param->acc.z)) { mi_param->acc.z += 0.01f; }

	// 速度を加算
	mi_param->vel.x *= missile.GetSpeed();
	mi_param->vel.y *= missile.GetSpeed();
	mi_param->vel.z *= missile.GetSpeed();

	// 位置を更新
	mi_param->pos.x += mi_param->vel.x;
	mi_param->pos.y += mi_param->vel.y;
	mi_param->pos.z += mi_param->vel.z;

	// 位置を反映
	missile.SetPos(mi_param->pos);

	missile.SetRot(LookAt(mi_param->vel));
}
