#pragma once
#include <DirectXMath.h>

/// <summary>
/// �p�[�e�B�N���̕`��ɕK�v�ȋ��ʃf�[�^
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
