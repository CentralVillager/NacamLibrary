#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include <memory>
#include <wrl.h>
#include "Emitter.h"
#include <DirectXMath.h>

class ParticleDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// テストパーティクル
	std::unique_ptr<Emitter> emitter_1_;
	std::unique_ptr<Emitter> emitter_2_;

	// ImGui用受け箱
	ParticleMember p_;
	ParticleMember p_2_;
	XMFLOAT3 range_;
	int generate_num_;

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
