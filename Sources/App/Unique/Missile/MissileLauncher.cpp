#include "MissileLauncher.h"

using namespace std;

MissileLauncher::MissileLauncher()
{}

bool MissileLauncher::FireMissile(MissileType type, const XMFLOAT3 &launch_pos, MissileManager *ptr)
{
	using enum MissileType;

	switch (type)
	{
	case Multi:

		mi_state_ = make_shared<MissileStateMulti>();
		break;

	case Charge:

		mi_state_ = make_shared<MissileStateCharge>();
		break;

	case Ultimate:

		mi_state_ = make_shared<MissileStateUltimate>();
		break;

	default:
		break;
	}

	return mi_state_->FireMissile(launch_pos, ptr);
}
