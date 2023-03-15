#pragma once
#include <DirectXMath.h>
#include "../../Lib/Object3d/Object3d.h"
#include "../../Lib/Model/Model.h"
#include "../../Lib/IndirectObject3d/IndirectObject3d.h"
#include "../../Lib/PlatePoly/PlatePoly.h"
#include "../../Lib/Sprite/NcmSprite.h"

// パーティクル一粒の構成要素
struct ParticleDesc
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 position_;	// 座標
	XMFLOAT3 velocity_;	// 速度
	XMFLOAT3 accel_;	// 加速度
	uint32_t life_;		// 寿命
	uint32_t frame_;	// 現在フレーム
	float s_scale_;		// 開始時スケール
	float e_scale_;		// 終了時スケール
	float scale_;		// スケール
	bool is_dead_;		// 死亡フラグ
	XMFLOAT3 color_;	// 色
	float alpha_;		// 透明度
	bool is_used_;		// 使われているか
	ncm_thandle tex_handle_;	// テクスチャハンドル
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

	// パーティクル一粒
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
