#pragma once
#include <DirectXMath.h>
#include "../../Lib/Math/NcmMath.h"
#include "../../Lib/Utility/NcmUtil.h"
#include "../../Lib/NacamError/NacamError.h"

class Missile;

/// <summary>
/// �~�T�C���ǔ��p�^�[���̊��N���X
/// </summary>
class IMissileHomingState
{
public:

	virtual void HomingTarget(Missile &missile) = 0;
};
