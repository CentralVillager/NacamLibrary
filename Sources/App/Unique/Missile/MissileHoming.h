#pragma once
#include <memory>
#include "IMissileHomingState.h"
#include "MissileHomingStateHighAccuracy.h"
#include "MissileHomingStateMiddleAccuracy.h"
#include "MissileHomingStateLowAccuracy.h"
#include "MissileDescs.h"

class MissileHoming
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	std::shared_ptr<IMissileHomingState> homing_accuracy_;

public:

	/// <summary>
	/// ターゲットを追尾する
	/// </summary>
	/// <param name="accuracy">追尾精度</param>
	/// <param name="dest_pos">ターゲットの位置</param>
	void HomingTarget(Missile &missile, HomingAccuracy accuracy)
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

		homing_accuracy_->HomingTarget(missile);
	}
};
