#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Sources/Lib/Sprite/NcmSprite.h"
//#include "../../Lib/NcmSprite/NcmSprite.h"

class Reticle
{
	using float2 = DirectX::XMFLOAT2;
	using float3 = DirectX::XMFLOAT3;

	//std::unique_ptr<NcmSprite> reticle_;
	static int reticle_;

public:

	Reticle();
	~Reticle();

public:

	static void LoadResources();

	void Initialize();
	void Update();
	void Draw();
};
