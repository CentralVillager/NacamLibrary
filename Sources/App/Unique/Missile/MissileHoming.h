#pragma once
#include <memory>
#include "IMissileHomingState.h"

/// <summary>
/// 追尾の精度
/// </summary>
enum class HomingAccuracy
{
	High,
	Middle,
	Low
};

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
	void HomingEnemy(HomingAccuracy accuracy, XMFLOAT3 dest_pos);
};
