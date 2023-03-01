#include "GridFloorManager.h"

GridFloorManager::GridFloorManager() :
	grid_floor_()
{}

GridFloorManager::~GridFloorManager()
{}

void GridFloorManager::Initialize(const UINT one_floor_line_num, const UINT line_offset, const XMFLOAT3 origin_pos)
{
	float z_size = 0;
	XMFLOAT3 pos = origin_pos;

	// 全てのフロアに対して
	for (auto &i : grid_floor_)
	{
		// 連続配置用に、z位置にグリッドサイズを加算
		pos.z += z_size;

		// 初期化
		i.Initialize(one_floor_line_num, line_offset, pos);
		z_size = i.GetZSize();
	}
}

void GridFloorManager::Update(float player_z_pos, float move_speed)
{
	for (auto &i : grid_floor_)
	{
		i.Update(move_speed);

		// グリッドの終端
		float finish_pos = i.GetPos().z + (i.GetZSize() / 2);

		// グリッドの終点がプレイヤーの後ろに到達したら
		if (finish_pos < player_z_pos)
		{
			float pos = finish_pos + i.GetZSize();
			i.SetZPos(pos);
			//i.SetZPos(finish_pos + i.GetZSize());

			int a = 0;
		}
	}
}

void GridFloorManager::Draw()
{
	for (auto &i : grid_floor_)
	{
		i.Draw();
	}
}
