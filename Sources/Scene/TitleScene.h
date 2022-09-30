#pragma once
#include <memory>
#include <DirectXMath.h>
#include "AbstractScene.h"
#include "../Camera/Camera.h"
#include "../Sprite/Sprite.h"
#include "../Model/Model.h"
#include "../Object3d/Object3d.h"
#include "../Particle/Emitter.h"

class TitleScene : public AbstractScene
{
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

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

	int logo_;

public:

	TitleScene();
	~TitleScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
