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
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Sprite> title_;

	std::unique_ptr<Model> model_sky_dome_;
	std::unique_ptr<Object3d> sky_dome_;

	std::unique_ptr<Emitter> emitter_;

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
