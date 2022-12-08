#pragma once
#include <DirectXMath.h>

/// <summary>
/// パーティクルの描画に必要な共通データ
/// </summary>
struct NcmParticleCommonArgs
{
	DirectX::XMFLOAT3 pos;
	float scale;
	float alpha;

	NcmParticleCommonArgs() :
		pos(),
		scale(),
		alpha()
	{}
};
