#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>
#include "../AbstractScene.h"
#include "../../Camera/Camera.h"
#include "../../Model/Model.h"
#include "../../Object3d/Object3d.h"
#include "../../IndirectObject3d/IndirectObject3d.h"
#include "../../Particle/Emitter.h"

class ExecuteIndirectDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// オブジェクト
	std::unique_ptr<IndirectObject3d> indirect_obj_;

public:

	ExecuteIndirectDemoScene();
	~ExecuteIndirectDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
