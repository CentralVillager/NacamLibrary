#include "MissileHomingStateNoHoming.h"
#include "Missile.h"
#include "MissileDescs.h"
#include "../../Lib/Math/NcmMath.h"

void MissileHomingStateNoHoming::HomingTarget(Missile &missile)
{
	MissileParam *mi_param = &missile.GetMissileParam();
	mi_param->accuracy_type = HomingAccuracy::NoHoming;

	mi_param->vel.x *= mi_param->speed;
	mi_param->vel.y *= mi_param->speed;
	mi_param->vel.z *= mi_param->speed;

	// 進行方向へ回頭させる
	DirectX::XMFLOAT3 rot = missile.GetRot();
	rot = NcmMath::LookAt(mi_param->vel);
	missile.SetRot(rot);
}
