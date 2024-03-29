#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateNoHoming : public IMissileHomingState
{
	// IMissileHomingState を介して継承されました
	virtual void HomingTarget(Missile &missile) override;
};
