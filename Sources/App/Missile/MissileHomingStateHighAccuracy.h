#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateHighAccuracy : public IMissileHomingState
{
	// IMissileHomingState を介して継承されました
	virtual void HomingTarget(Missile &missile) override;
};
