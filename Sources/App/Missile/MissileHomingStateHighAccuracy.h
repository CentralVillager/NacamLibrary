#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateHighAccuracy : public IMissileHomingState
{
	// IMissileHomingState ����Čp������܂���
	virtual void HomingTarget(Missile &missile) override;
};
