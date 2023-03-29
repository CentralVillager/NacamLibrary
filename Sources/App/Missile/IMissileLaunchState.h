#pragma once
#include <DirectXMath.h>
#include "../Missile/MissileManager.h"
#include "../../Lib/Utility/NcmUtil.h"
#include "../../Lib/NacamError/NacamError.h"
#include "MissileDescs.h"

/// <summary>
/// ミサイル発射パターンの基底クラス
/// </summary>
class IMissileLaunchState
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	virtual bool FireMissile(LaunchedBy launcher, const XMFLOAT3 &launch_pos, MissileManager *ptr) = 0;
};
