#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Sprite/Sprite.h"

class Reticle
{
	using float2 = DirectX::XMFLOAT2;
	using float3 = DirectX::XMFLOAT3;

	std::unique_ptr<Sprite> reticle_;

public:

	Reticle();
	~Reticle();

public:

	static void LoadResources();

	void Initialize();
	void Update();
	void Draw();
};
