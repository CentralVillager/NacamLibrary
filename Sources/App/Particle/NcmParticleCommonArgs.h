#pragma once
#include <DirectXMath.h>

/// <summary>
/// �p�[�e�B�N���̕`��ɕK�v�ȋ��ʃf�[�^
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
