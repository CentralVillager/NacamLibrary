#pragma once
#include <DirectXMath.h>
#include "NcmUtil.h"

namespace NcmColor
{
	constexpr DirectX::XMFLOAT3 TRAIL_RED = DirectX::XMFLOAT3(0.9f * 0.1f, 0.4f * 0.1f, 0.4f * 0.1f);
	constexpr DirectX::XMFLOAT3 TRAIL_WHITE = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	const DirectX::XMFLOAT3 EXPLOSION_ORENGE = DirectX::XMFLOAT3(NcmUtill::Convert256to01(0xe6), NcmUtill::Convert256to01(0x5c), NcmUtill::Convert256to01(0x00));
	const DirectX::XMFLOAT3 BLACK = DirectX::XMFLOAT3(NcmUtill::Convert256to01(0x32), NcmUtill::Convert256to01(0x32), NcmUtill::Convert256to01(0x32));
}
