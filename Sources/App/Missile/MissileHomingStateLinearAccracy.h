#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateLinearAccracy : public IMissileHomingState
{
	// IMissileHomingState を介して継承されました
	virtual void HomingTarget(Missile &missile) override;
};
