#pragma once
#include <DirectXMath.h>
#include "../../Lib/Object3d/Object3d.h"
#include "../../Lib/Model/Model.h"
#include "../../Lib/IndirectObject3d/IndirectObject3d.h"
#include "../../Lib/PlatePoly/PlatePoly.h"
#include "../../Lib/Sprite/NcmSprite.h"

// �p�[�e�B�N���ꗱ�̍\���v�f
struct ParticleDesc
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 position_;	// ���W
	XMFLOAT3 velocity_;	// ���x
	XMFLOAT3 accel_;	// �����x
	uint32_t life_;		// ����
	uint32_t frame_;	// ���݃t���[��
	float s_scale_;		// �J�n���X�P�[��
	float e_scale_;		// �I�����X�P�[��
	float scale_;		// �X�P�[��
	bool is_dead_;		// ���S�t���O
	XMFLOAT3 color_;	// �F
	float alpha_;		// �����x
	bool is_used_;		// �g���Ă��邩
	ncm_thandle tex_handle_;	// �e�N�X�`���n���h��
	uint32_t id_;

	ParticleDesc() :
		position_(),
		velocity_(),
		accel_(),
		life_(),
		frame_(),
		s_scale_(),
		e_scale_(),
		scale_(),
		is_dead_(),
		color_(),
		alpha_(1.0f),
		is_used_(),
		tex_handle_()
	{}
};

class Particle
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// �p�[�e�B�N���ꗱ
	std::shared_ptr<ParticleDesc> particle_;

public:

	Particle();
	~Particle();

public:

	inline const XMFLOAT3 &GetPos() { return particle_->position_; }
	inline const float &GetScale() { return particle_->scale_; }
	inline const uint32_t GetLife() { return particle_->life_; }
	inline const XMFLOAT3 &GetColor() { return particle_->color_; }
	inline const float GetAlpha() { return particle_->alpha_; }
	inline const bool &GetIsDead() { return particle_->is_dead_; }
	inline const bool &GetIsUsed() { return particle_->is_used_; }
	inline const uint32_t GetId() { return particle_->id_; }
	inline void SetIsUsed(const bool is_used) { particle_->is_used_ = is_used; }
	inline void SetParticleValue(const ParticleDesc &p) { *particle_ = p; }

public:

	void Initialize(Model *model, const ParticleDesc &part_desc_);
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

public:

	void ResetParamater();
};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs);
const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 &rhs);
const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3 &lhs, const int &rhs);
