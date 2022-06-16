#pragma once
#include <DirectXMath.h>
#include "Object3d.h"
#include "Model.h"

// �p�[�e�B�N���ꗱ�̍\���v�f
struct ParticleMember {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// ���W
	XMFLOAT3 position_ = {};

	// ���x
	XMFLOAT3 velocity_ = {};

	// �����x
	XMFLOAT3 accel_ = {};

	// �X�P�[��
	float scale_;

	// �J�n���X�P�[��
	float s_scale_;

	// �I�����X�P�[��
	float e_scale_;

	// ���݃t���[��
	int frame_ = 0;

	// �I���t���[��
	int num_frame_ = 0;

	// ���S�t���O
	bool is_dead_ = false;

};

class Particle {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::unique_ptr<Object3d> object_;

private:

	std::shared_ptr<ParticleMember> particle_;

public:

	void Initialize(Model *model, ParticleMember &particle, int life);
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

	inline const bool &GetIsDead() { return particle_->is_dead_; }
	inline void SetParticleValue(const ParticleMember &p) { *particle_ = p; }
};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs);