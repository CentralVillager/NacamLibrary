#pragma once
#include <DirectXMath.h>

/// <summary>
/// パーティクルの描画に必要な共通データ
/// </summary>
struct NcmParticleCommonArgs
{
	DirectX::XMFLOAT3 pos;
	float scale;
	/*uint32_t life;
	bool is_dead;*/

	NcmParticleCommonArgs() :
		pos(),
		scale()/*,
		life(),
		is_dead(true)*/
	{}
};
