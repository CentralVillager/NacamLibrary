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

		if (mi_param->type == MissileType::ForEnemy)
		{
			if (len.m128_f32[0] <= GenerateRandom(5.0f, 50.0f))
			{
				// エミッターの終了準備
				missile.PrepareTermEmitter();
				// ミサイルの寿命を強制的に0に
				missile.SetMissileLife(0);
				// ミサイルを無効化(死亡フラグは建てない)
				missile.InvalidateMissile();
			}
		}
	}
};
