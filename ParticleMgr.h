#pragma once
#include "Particle.h"
#include <forward_list>
#include "Model.h"
#include <memory>
#include <DirectXMath.h>

class ParticleMgr {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// ���f���f�[�^
	std::unique_ptr<Model> model_;

	// �p�[�e�B�N���Ǘ��R���e�i
	std::forward_list<Particle> particles_;

	ParticleMember p_;

	int interval_ = 0;

public:

	void Initialize(int num);
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

public:

	void Add(int life, ParticleMember p);

	ParticleMember GenerateValue();
	ParticleMember GenerateValue(XMFLOAT3 accel, int life);
};
