#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateNoHoming : public IMissileHomingState
{
	// IMissileHomingState ����Čp������܂���
	virtual void HomingTarget(Missile &missile) override;
};
