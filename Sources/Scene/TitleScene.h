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
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Sprite> title_;
	std::unique_ptr<Sprite> play_b_;
	std::unique_ptr<Sprite> play_w_;
	std::unique_ptr<Sprite> exit_b_;
	std::unique_ptr<Sprite> exit_w_;
	std::unique_ptr<Sprite> space_;

	std::unique_ptr<Model> model_sky_dome_;
	std::unique_ptr<Object3d> sky_dome_;

	std::unique_ptr<Emitter> emitter_;

	XMFLOAT2 ImGui_pos_;
	XMFLOAT2 ImGui_pos2_;

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