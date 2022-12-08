#pragma once
#include <memory>
#include <forward_list>
#include "../../App/Particle/Emitter.h"
#include "../../Lib/PlatePoly/NcmPlatePoly.h"
#include "NcmParticleCommonArgs.h"

/// <summary>
/// �|�����g�p�����p�[�e�B�N���̃}�l�[�W��
/// </summary>
class NcmParticleManager
{
	static std::forward_list<NcmParticleCommonArgs> part_args_;	// �`��p���ʃf�[�^
	static size_t particle_num;
	static size_t max_particle_num;

	std::unique_ptr<NcmPlatePoly> poly_;	// �`��f�[�^

public:

	NcmParticleManager();
	~NcmParticleManager();

public:

	/// <summary>
	/// �V���Ƀp�[�e�B�N���̗v�f��ǉ�
	/// </summary>
	/// <param name="args"></param>
	static void AddParticleCommonArgs(std::forward_list<NcmParticleCommonArgs> &args);

	/// <summary>
	/// �i�[����Ă����v�f��S�폜
	/// </summary>
	static void ClearParticleArgsBeforeUpdate();

public:

	void Initialize();
	void Update();
	void Draw();
	static void StaticDebugDraw();
};
