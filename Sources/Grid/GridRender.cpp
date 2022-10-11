#include "../Grid/GridRender.h"
#include "../Utility/Utility.h"

Camera *GridRender::cam_ptr_ = nullptr;

void GridRender::Initialize(const UINT &grid_num, const UINT &offset, const XMFLOAT3 &origin_pos)
{
	Line::SetCamera(cam_ptr_);

	// +2 しないとグリッドの終端が中途半端に終わります
	grid_.resize(grid_num + 2);

	for (auto &i : grid_)
	{
		i.Initialize();
	}

	// x軸に対してz軸方向に展開
	float x_pos = (origin_pos.x - (grid_num / 2) * offset) / 2;
	float z_pos = (origin_pos.z + (grid_num / 2) * offset) / 2;
	for (UINT i = 0; i < grid_.size() / 2; i++)
	{
		grid_[i].SetVertPos(XMFLOAT3(offset * i + x_pos, 0.0f, -z_pos), (UINT)(VertDesc::Start));
		grid_[i].SetVertPos(XMFLOAT3(offset * i + x_pos, 0.0f, z_pos), (UINT)(VertDesc::End));
	}

	// z軸に対してx軸方向に展開
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
