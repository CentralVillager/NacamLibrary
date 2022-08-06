#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include "../Line/Line.h"
#include "../Camera/Camera.h"

class GridRender {
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	static Camera *cam_ptr_;
	std::vector<Line> grid_;

public:

	static void SetCamera(Camera *camera) { cam_ptr_ = camera; }

	void Initialize(UINT grid_num, UINT offset, XMFLOAT3 origin_pos);
	void Update();
	void Draw();
	void DebugDraw();
};
