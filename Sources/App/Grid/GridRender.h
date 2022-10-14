#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include "../../Lib/Line/Line.h"
#include "../../Lib/Camera/Camera.h"

class GridRender
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	static Camera *cam_ptr_;
	std::vector<Line> grid_;

public:

	static void SetCamera(Camera *camera) { cam_ptr_ = camera; }

	void Initialize(const UINT &grid_num, const UINT &offset, const XMFLOAT3 &origin_pos);
	void Update();
	void Draw();
	void DebugDraw();

public:

	void MoveMinusZ();
};
