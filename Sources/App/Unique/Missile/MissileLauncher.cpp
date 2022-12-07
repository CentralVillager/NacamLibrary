#include "MissileLauncher.h"
#include "MissileStateMulti.h"
#include "MissileStateCharge.h"
#include "MissileStateUltimate.h"

using namespace std;

bool MissileLauncher::FireMissile(MissileType type, const XMFLOAT3 &launch_pos, MissileManager *ptr)
{
	using enum MissileType;

	switch (type)
	{
	case Multi:

		state_ = make_shared<MissileStateMulti>();
		break;

	case Charge:

		state_ = make_shared<MissileStateCharge>();
		break;

	case Ultimate:

		state_ = make_shared<MissileStateUltimate>();
		break;

	default:
		break;
	}

	return state_->FireMissile(launch_pos, ptr);
}
