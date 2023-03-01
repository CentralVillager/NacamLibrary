#pragma once
#include "IMissileHomingState.h"

class MissileHomingStateNoHoming : public IMissileHomingState
{
	// IMissileHomingState ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void HomingTarget(Missile &missile) override;
};
