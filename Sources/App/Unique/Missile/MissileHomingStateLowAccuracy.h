#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateLowAccuracy : public IMissileHomingState
{
	// IMissileHomingState ����Čp������܂���
	virtual void HomingTarget(Missile &missile) override;
};
