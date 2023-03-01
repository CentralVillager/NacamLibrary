#pragma once
#include <memory>
#include <array>
#include <DirectXMath.h>
#include "GridRender.h"

/// <summary>
/// グリッドで描かれたフロアを管理するクラス
/// </summary>
class GridFloorManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// 床の数
	static constexpr uint32_t ALL_FLOOR_NUM_ = 2;

private:

	// グリッドで描かれた床
	std::array<GridRender, ALL_FLOOR_NUM_> grid_floor_;

public:

	GridFloorManager();
	~GridFloorManager();

public:

	void Initialize(const UINT one_floor_line_num, const UINT line_offset, const XMFLOAT3 origin_pos);
	void Update(float player_z_pos, float move_speed);
	void Draw();
};
