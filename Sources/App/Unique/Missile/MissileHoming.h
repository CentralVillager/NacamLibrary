#pragma once
#include <memory>
#include "IMissileHomingState.h"

/// <summary>
/// �ǔ��̐��x
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
	/// �^�[�Q�b�g��ǔ�����
	/// </summary>
	/// <param name="accuracy">�ǔ����x</param>
	/// <param name="dest_pos">�^�[�Q�b�g�̈ʒu</param>
	void HomingEnemy(HomingAccuracy accuracy, XMFLOAT3 dest_pos);
};
