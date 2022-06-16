#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include <memory>
#include <wrl.h>
#include "ParticleMgr.h"

class ParticleDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// テストパーティクル
	std::unique_ptr<ParticleMgr> test_particle_;

public:

	ParticleDemoScene();
	~ParticleDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
