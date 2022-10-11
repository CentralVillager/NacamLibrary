#include "../Grid/GridRender.h"
#include "../Utility/Utility.h"

Camera *GridRender::cam_ptr_ = nullptr;

void GridRender::Initialize(const UINT &grid_num, const UINT &offset, const XMFLOAT3 &origin_pos)
{
	Line::SetCamera(cam_ptr_);

	// +2 ���Ȃ��ƃO���b�h�̏I�[�����r���[�ɏI���܂�
	grid_.resize(grid_num + 2);

	for (auto &i : grid_)
	{
		i.Initialize();
	}

	// x���ɑ΂���z�������ɓW�J
	float x_pos = (origin_pos.x - (grid_num / 2) * offset) / 2;
	float z_pos = (origin_pos.z + (grid_num / 2) * offset) / 2;
	for (UINT i = 0; i < grid_.size() / 2; i++)
	{
		grid_[i].SetVertPos(XMFLOAT3(offset * i + x_pos, 0.0f, -z_pos), (UINT)(VertDesc::Start));
		grid_[i].SetVertPos(XMFLOAT3(offset * i + x_pos, 0.0f, z_pos), (UINT)(VertDesc::End));
	}

	// z���ɑ΂���x�������ɓW�J
	z_pos = (origin_pos.z - (grid_num / 2) * offset) / 2;
	for (UINT i = 0; i < grid_.size() / 2; i++)
	{
		grid_[i + grid_.size() / 2].SetVertPos(XMFLOAT3(-x_pos, 0.0f, offset * i + z_pos), (UINT)(VertDesc::Start));
		grid_[i + grid_.size() / 2].SetVertPos(XMFLOAT3(x_pos, 0.0f, offset * i + z_pos), (UINT)(VertDesc::End));
	}
}

void GridRender::Update()
{
	for (auto &i : grid_)
	{
		i.Update();
	}
}

void GridRender::Draw()
{
	for (auto &i : grid_)
	{
		i.Draw();
	}
}

void GridRender::DebugDraw()
{}

void GridRender::MoveMinusZ()
{
	/*for (UINT i = 0; i < grid_.size(); i++)
	{
		float pos_z = grid_[i].GetPosition().z;
		pos_z -= 0.5f;
		grid_[i].SetPosition(XMFLOAT3(0, 0, pos_z));
	}*/

	for (auto &i : grid_)
	{
		float pos_z = i.GetPosition().z;
		pos_z -= 0.5f;
		i.SetPosition(XMFLOAT3(0, 0, pos_z));
	}
}
