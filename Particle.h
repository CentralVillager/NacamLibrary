#pragma once
#include <DirectXMath.h>
#include "Object3d.h"
#include "Model.h"

// パーティクル一粒の構成要素
struct ParticleMember {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// 座標
	XMFLOAT3 position_ = {};

	// 速度
	XMFLOAT3 velocity_ = {};

	// 加速度
	XMFLOAT3 accel_ = {};

	// スケール
	float scale_;

	// 開始時スケール
	float s_scale_;

	// 終了時スケール
	float e_scale_;

	// 現在フレーム
	int frame_ = 0;

	// 終了フレーム
	int num_frame_ = 0;

	// 死亡フラグ
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