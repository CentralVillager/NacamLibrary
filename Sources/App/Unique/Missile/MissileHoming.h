#pragma once
#include <memory>
#include "IMissileHomingState.h"
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
	void HomingTarget(HomingAccuracy accuracy, XMFLOAT3 &dest_pos);
};
