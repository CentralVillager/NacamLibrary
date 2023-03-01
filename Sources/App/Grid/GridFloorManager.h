#pragma once
#include <memory>
#include <array>
#include <DirectXMath.h>
#include "GridRender.h"

/// <summary>
/// �O���b�h�ŕ`���ꂽ�t���A���Ǘ�����N���X
/// </summary>
class GridFloorManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// ���̐�
	static constexpr uint32_t ALL_FLOOR_NUM_ = 2;

private:

	// �O���b�h�ŕ`���ꂽ��
	std::array<GridRender, ALL_FLOOR_NUM_> grid_floor_;

public:

	GridFloorManager();
	~GridFloorManager();

public:

	void Initialize(const UINT one_floor_line_num, const UINT line_offset, const XMFLOAT3 origin_pos);
	void Update(float player_z_pos, float move_speed);
	void Draw();
};
