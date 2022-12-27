#pragma once
#include <DirectXMath.h>
#include "Missile.h"
#include "MissileDescs.h"
#include "../../../App/Math/NcmMath.h"
#include "../../../App/Utility/NcmUtil.h"
#include "../../../Lib/NacamError/NacamError.h"

class Missile;

/// <summary>
/// �~�T�C���ǔ��p�^�[���̊��N���X
/// </summary>
class IMissileHomingState
{
public:

	virtual void HomingTarget(Missile &missile) = 0;
};
