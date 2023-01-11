#pragma once
#include "Particle.h"
#include <forward_list>
#include <list>
#include <memory>
#include <DirectXMath.h>
#include "../Sources/Lib/Model/Model.h"
#include "../../Lib/Sprite/NcmSprite.h"
#include "NcmParticleCommonArgs.h"

// �G�~�b�^�[�̐ݒ�\����
struct EmitterDesc
{
	ParticleDesc part_desc_;		// �p�[�e�B�N���̍\���v�f
	DirectX::XMFLOAT3 pos_rand_;	// �����ʒu�̗����̐U�ꕝ
	DirectX::XMFLOAT3 vel_rand_;	// ���x�̗����̐U�ꕝ
	UINT gene_num_;					// ���������
	bool use_life_;					// �G�~�b�^�[�̎�����L���ɂ��邩
	UINT life_;						// �G�~�b�^�[�̎���
	bool is_dead_;					// ����ł��邩
	ncm_thandle tex_handle_;		// �e�N�X�`���n���h��

	EmitterDesc() :
		part_desc_(),
		pos_rand_(),
		vel_rand_(),
		gene_num_(),
		use_life_(),
		life_(1),
		is_dead_(),
		tex_handle_()
	{}
};

class Emitter
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// ���炩���ߊm�ۂ��鐔
	static constexpr uint32_t RESERVE_NUM_ = 512;

private:

	// ���f���f�[�^
	static std::unique_ptr<Model> model_;

	// �e�N�X�`���n���h��
	static ncm_thandle tex_handle_;

	// ���\�[�X�V�F�A�Ńp�[�e�B�N���Ǘ��R���e�i
	static std::list<Particle> shared_particles_;

	// �g�p���ꂽ�p�[�e�B�N���̐�
	static uint32_t particle_num_;

private:

	// �p�[�e�B�N���Ǘ��R���e�i
	std::forward_list<Particle> particles_;

	// �p�[�e�B�N�������ɕK�v�ȗv�f
	EmitterDesc emitter_desc_;

	// �p�[�e�B�N�����g�p���ꂽ���Ɋւ���_�[�e�B�t���O
	bool updated_dirty_ = false;

public:

	// �A�N�Z�b�T
	inline const EmitterDesc &GetEmitterDesc() { return emitter_desc_; }
	inline const XMFLOAT3 &GetPosition() { return emitter_desc_.part_desc_.position_; }

	inline void SetEmitterDesc(const EmitterDesc &p) { emitter_desc_ = p; }
	inline void SetPosition(const XMFLOAT3 &pos) { emitter_desc_.part_desc_.position_ = pos; }

public:

	/// <summary>
	/// ������
	/// </summary>
	static void LoadResources();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �m�ۂ��ꂽ�p�[�e�B�N�����g��
	/// </summary>
	void UseParticle(std::list<Particle>::iterator last_itr);

	/// <summary>
	/// �p�[�e�B�N���𐶐�����
	/// </summary>
	void GenerateParticle();

	/// <summary>
	/// �p�[�e�B�N�����X�V����
	/// </summary>
	void UpdateParticle();

	/// <summary>
	/// �G�~�b�^�[�I������������
	/// </summary>
	void PrepareTerminate();

	/// <summary>
	/// �G�~�b�^�[���I���o���邩��m�点��
	/// </summary>
	/// <returns>��</returns>
	bool NoticeCanTerminate();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �f�o�b�O�p�`��
	void DebugDraw();
	static void StaticDebugDraw();

private:

	// �R���e�i�ɗ���ǉ�
	void Add(const ParticleDesc &p);

	// �l�𐶐�����
	ParticleDesc GenerateValue(const EmitterDesc &emitter);
};