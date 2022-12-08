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

	// 座標
	XMFLOAT3 position_ = {};
	// 速度
	XMFLOAT3 velocity_ = {};
	// 加速度
	XMFLOAT3 accel_ = {};
	// 開始時スケール
	float s_scale_;
	// 終了時スケール
	float e_scale_ = 0.0f;
	// スケール
	float scale_;
	// 終了フレーム
	int life_ = 0;
	// 死亡フラグ
	bool is_dead_ = false;
	// 現在フレーム
	int frame_ = 0;
	// 透明度
	float alpha_ = 1.0f;
	// テクスチャハンドル
	ncm_thandle tex_handle_;
	// 使われているか
	bool is_used = false;
};

class Particle
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::unique_ptr<Object3d> object_;
	std::shared_ptr<ParticleDesc> particle_;

	std::unique_ptr<PlatePoly> plate_;

public:

	Particle();
	~Particle();

public:

	inline const XMFLOAT3 &GetPos() { return particle_->position_; }
	inline const float &GetScale() { return particle_->scale_; }
	inline const uint32_t GetLife() { return (uint32_t)(particle_->life_); }
	inline const float GetAlpha() { return particle_->alpha_; }
	inline const bool &GetIsDead() { return particle_->is_dead_; }
	inline const bool &GetIsUsed() { return particle_->is_used; }
	inline void SetIsUsed(const bool is_used) { particle_->is_used = is_used; }
	inline void SetParticleValue(const ParticleDesc &p) { *particle_ = p; }

public:

	void Initialize(Model *model, const ParticleDesc &particle);
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
