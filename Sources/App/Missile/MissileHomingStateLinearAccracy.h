#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateLinearAccracy : public IMissileHomingState
{
	// IMissileHomingState ����Čp������܂���
	virtual void HomingTarget(Missile &missile) override;
};
