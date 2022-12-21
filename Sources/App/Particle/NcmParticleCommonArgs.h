#pragma once
#include <DirectXMath.h>

/// <summary>
/// パーティクルの描画に必要な共通データ
/// </summary>
struct NcmParticleCommonArgs
{
	DirectX::XMFLOAT3 pos;
	float scale;
	DirectX::XMFLOAT3 color;
	float alpha;

	NcmParticleCommonArgs() :
		pos(),
		scale(),
		color(),
		alpha()
	{}
};
