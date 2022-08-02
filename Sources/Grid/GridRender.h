#pragma once
#include "../Line/Line.h"
#include <DirectXMath.h>
#include <array>
#include "../Camera/Camera.h"
#include <memory>

class GridRender {
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	static const UINT GRID_NUM_ = 100;
	static const UINT GRID_HALF_NUM_ = GRID_NUM_ / 2;
	static const UINT OFFSET_ = 10;

private:

	static Camera *cam_ptr_;
	std::array<Line, GRID_NUM_> grid_x_;
	std::array<Line, GRID_NUM_> grid_z_;

public:

	static void SetCamera(Camera *camera) { cam_ptr_ = camera; }

	void Initialize(XMFLOAT3 pos);
	void Update();
	void Draw();
};
