#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include <memory>
#include <wrl.h>
#include "Emitter.h"
#include <DirectXMath.h>
#include <forward_list>

class ParticleDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	enum class MODE {

		MONO,
		MISSILE,
		MISSILE_CONTAINER
	};

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Object3d> sky_dome_;
	std::unique_ptr<Model> model_sky_dome_;

	// テストパーティクル
	std::unique_ptr<Emitter> contrail_1_;
	std::unique_ptr<Emitter> contrail_2_;
	std::unique_ptr<Emitter> emitter_1_;

	std::forward_list<Emitter> contrails_1_;

	MODE mode_;

	int emitter_life_;

public:

	ParticleDemoScene();
	~ParticleDemoScene();

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;

public:

	void ResetPos();
	void MoveZ();
};
