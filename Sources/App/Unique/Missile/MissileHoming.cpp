#include "MissileHoming.h"
#include "MissileHomingStateHighAccuracy.h"
#include "MissileHomingStateMiddleAccuracy.h"
#include "MissileHomingStateLowAccuracy.h"

void MissileHoming::HomingTarget(HomingAccuracy accuracy, XMFLOAT3 dest_pos)
{
	using enum HomingAccuracy;

	switch (accuracy)
	{
	case High:

		homing_accuracy_ = std::make_shared<MissileHomingStateHighAccuracy>();
		break;

	case Middle:

		homing_accuracy_ = std::make_shared<MissileHomingStateMiddleAccuracy>();
		break;

	case Low:

		homing_accuracy_ = std::make_shared<MissileHomingStateLowAccuracy>();
		break;

	default:
		break;
	}

	homing_accuracy_->HomingTarget(dest_pos);
}
