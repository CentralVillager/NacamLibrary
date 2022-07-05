#pragma once
#include "Particle.h"
#include <forward_list>
#include "Model.h"
#include <memory>
#include <DirectXMath.h>

class Emitter {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// ���f���f�[�^
	static std::unique_ptr<Model> model_;

public:

	// �G�~�b�^�[�����ɕK�v�ȗv�f
	struct EmitterArgs {

		// �p�[�e�B�N���̍\���v�f
		ParticleMember particle;

		// �����ʒu�̗����̐U�ꕝ
		XMFLOAT3 pos_rand_;

		// ���x�̗����̐U�ꕝ
		XMFLOAT3 vel_rand_;

		// ���������
		UINT gene_num_;

		// �G�~�b�^�[�̎�����L���ɂ��邩
		bool use_life_;

		// �G�~�b�^�[�̎���
		UINT life_ = 1;

		bool is_dead_ = false;
	};

private:

	// �p�[�e�B�N���Ǘ��R���e�i
	std::forward_list<Particle> particles_;

	// �p�[�e�B�N�������ɕK�v�ȗv�f
	EmitterArgs emitter_args_;

public:

	// �A�N�Z�b�T
	inline const EmitterArgs &GetEmitterArgs() { return emitter_args_; }
	inline const XMFLOAT3 &GetPosition() { return emitter_args_.particle.position_; }

	inline void SetEmitterArgs(const EmitterArgs &p) { emitter_args_ = p; }
	inline void SetPosition(const XMFLOAT3 &pos) { emitter_args_.particle.position_ = pos; }

private:

	// �R���e�i�ɗ���ǉ�
	void Add(ParticleMember p);

	// �l�𐶐�����
	ParticleMember GenerateValue(EmitterArgs emitter);

public:

	/// <summary>
	/// ������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// �p�[�e�B�N���𐶐�����
	/// </summary>
	void GenerateParticle();

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
	/// </summary>
	void DebugDraw(const string &name);
	void DebugDraw();
};