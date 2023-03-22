#pragma once
#include <memory>
#include <forward_list>
#include <array>
#include "../../App/Particle/Emitter.h"
#include "../../Lib/3D/NcmPlatePoly.h"
#include "NcmParticleCommonArgs.h"

enum class ParticleType
{
	Trail,
	Explosion,
	MaxParticleTypeNum
};

/// <summary>
/// �|�����g�p�����p�[�e�B�N���̃}�l�[�W��
/// </summary>
class NcmParticleManager
{
	static std::forward_list<NcmParticleCommonArgs> part_args_;	// �`��p���ʃf�[�^
	static size_t particle_num;
	static size_t max_particle_num;

	std::unique_ptr<NcmPlatePoly> poly_;	// �`��f�[�^
	std::array<std::unique_ptr<NcmPlatePoly>, 
		(size_t)(ParticleType::MaxParticleTypeNum)> polies_;

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
	/// ���K��Update�̍ŏ��ɌĂяo��
	/// </summary>
	static void ClearParticleArgsBeforeUpdate();

public:

	void Initialize();
	void Update();
	void Draw();
	static void StaticDebugDraw();
};
