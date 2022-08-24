#pragma once
#include <DirectXMath.h>
#include "../Object3d/Object3d.h"
#include "../IndirectObject3d/IndirectObject3d.h"
#include "../Model/Model.h"

// �p�[�e�B�N���ꗱ�̍\���v�f
struct ParticleMember
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// ���W
	XMFLOAT3 position_ = {};

	// ���x
	XMFLOAT3 velocity_ = {};

	// �����x
	XMFLOAT3 accel_ = {};

	// �J�n���X�P�[��
	float s_scale_;

	// �I�����X�P�[��
	float e_scale_ = 0.0f;

	// �X�P�[��
	float scale_;

	// �I���t���[��
	int life_ = 0;

	// ���S�t���O
	bool is_dead_ = false;

	// ���݃t���[��
	int frame_ = 0;
};

class Particle
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::unique_ptr<Object3d> object_;
	std::shared_ptr<ParticleMember> particle_;

public:

	Particle();
	~Particle();

public:

	void Initialize(Model *model, ParticleMember &particle);
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

	inline const bool &GetIsDead() { return particle_->is_dead_; }
	inline void SetParticleValue(const ParticleMember &p) { *particle_ = p; }
};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs);
const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs);
const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3 &lhs, const int &rhs);
