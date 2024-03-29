#pragma once
#include <memory>
#include <DirectXMath.h>
#include "AbsScene.h"
#include "../Particle/Emitter.h"
#include "../../Lib/Camera/Camera.h"
#include "../../Lib/2D/NcmSprite.h"
#include "../../Lib/3D/Model.h"
#include "../../Lib/3D/Object3d.h"
#include "../../App/Particle/NcmParticleManager.h"

class TitleScene : public AbsScene
{
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	std::unique_ptr<Camera> camera_;

	int title_;
	int play_b_;
	int play_w_;
	int exit_b_;
	int exit_w_;
	int space_;

	std::unique_ptr<Model> model_sky_dome_;
	std::unique_ptr<Object3d> sky_dome_;

	std::unique_ptr<Emitter> emitter_;

	std::unique_ptr<NcmParticleManager> part_mgr_;

	int logo_;

	bool is_scene_change_;

public:

	TitleScene();
	~TitleScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void AfterPostEffectDraw() override;
	void DebugDraw() override;
};
