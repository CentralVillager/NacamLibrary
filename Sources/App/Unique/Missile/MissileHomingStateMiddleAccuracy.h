#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateMiddleAccuracy : public IMissileHomingState
{
	// IMissileHomingState ����Čp������܂���
	virtual void HomingTarget(Missile &missile) override;
};
