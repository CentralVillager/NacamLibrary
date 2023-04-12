#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include <forward_list>
#include "../AbsScene.h"
#include "../../Particle/Emitter.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../App/Particle/NcmParticleManager.h"

class ParticleDemoScene : public AbsScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	enum class MODE
	{
		MONO,
		MISSILE,
		MISSILE_CONTAINER
	};

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// 天球
	std::unique_ptr<Object3d> sky_dome_;

	// 天球モデル
	std::unique_ptr<Model> model_sky_dome_;

	// テストパーティクル
	std::unique_ptr<Emitter> contrail_1_;
	std::unique_ptr<Emitter> contrail_2_;
	std::unique_ptr<Emitter> emitter_1_;

	// エミッターコンテナ
	std::forward_list<Emitter> contrails_1_;

	std::unique_ptr<Emitter> dust_;

	std::unique_ptr<NcmParticleManager> particle_mgr_;

	// モード格納
	MODE mode_;

	// エミッターの寿命
	int emitter_life_;

	// リセットするか
	bool notice_reset_ = false;

	// エミッターを生成するか
	bool notice_generate_ = false;

	bool is_wire_ = true;

public:

	ParticleDemoScene();
	~ParticleDemoScene();

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void AfterPostEffectDraw() override;
	void DebugDraw() override;

public:

	void ResetParam();
	void ResetPos();
	void MoveZ();
};
