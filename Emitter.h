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

	// �p�[�e�B�N���Ǘ��R���e�i
	std::forward_list<Particle> particles_;

public:

	/// <summary>
	/// ������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// �p�[�e�B�N���𐶐�����
	/// </summary>
	/// <param name="p">�ꗱ�̏����l</param>
	/// <param name="rand_range">�ʒu�̃����_����</param>
	/// <param name="num_per_frame">�������鐔 / f</param>
	void GenerateParticle(ParticleMember p, XMFLOAT3 rand_range, int num_per_frame);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �f�o�b�O�p�`��
	/// </summary>
	void DebugDraw();

private:

	// �R���e�i�ɗ���ǉ�
	void Add(ParticleMember p);

	// �l�𐶐�����
	ParticleMember GenerateValue(ParticleMember p, XMFLOAT3 rand_range);
};
