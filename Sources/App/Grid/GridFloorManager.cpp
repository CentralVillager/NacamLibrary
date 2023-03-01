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

	// �S�Ẵt���A�ɑ΂���
	for (auto &i : grid_floor_)
	{
		// �A���z�u�p�ɁAz�ʒu�ɃO���b�h�T�C�Y�����Z
		pos.z += z_size;

		// ������
		i.Initialize(one_floor_line_num, line_offset, pos);
		z_size = i.GetZSize();
	}
}

void GridFloorManager::Update(float player_z_pos, float move_speed)
{
	for (auto &i : grid_floor_)
	{
		i.Update(move_speed);

		// �O���b�h�̏I�[
		float finish_pos = i.GetPos().z + (i.GetZSize() / 2);

		// �O���b�h�̏I�_���v���C���[�̌��ɓ��B������
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
