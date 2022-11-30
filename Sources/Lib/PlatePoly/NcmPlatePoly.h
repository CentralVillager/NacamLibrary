#pragma once
#include <DirectXMath.h>
#include "../Sprite/NcmSprite.h"
#include "../Camera/Camera.h"

class NcmPlatePoly : public NcmSprite
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	// カメラのポインタ
	static Camera *cam_ptr_;

public:

	static void SetCamera(Camera *cam) { cam_ptr_ = cam; }

	static inline void SetPos(const ncm_thandle handle, const XMFLOAT3 &pos)
	{
		sprite_hub_[handle].pos_ = pos;
		TransferVertices(&sprite_hub_[handle]);
	}

public:

	static void Update(const ncm_thandle handle);
	static void Draw(const ncm_thandle handle);
};
