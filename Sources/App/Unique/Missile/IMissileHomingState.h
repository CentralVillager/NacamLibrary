#pragma once
#include <DirectXMath.h>
#include "Missile.h"
#include "../../../Lib/NacamError/NacamError.h"
#include "../../../App/Math/NcmMath.h"
#include "../../../App/Utility/NcmUtil.h"

/// <summary>
/// ミサイル追尾パターンの基底クラス
/// </summary>
class IMissileHomingState
{
protected:

	using XMFLOAT3 = DirectX::XMFLOAT3;

protected:

	Missile *missile_;

public:

	virtual void HomingTarget(XMFLOAT3 dest_pos) = 0;
};
