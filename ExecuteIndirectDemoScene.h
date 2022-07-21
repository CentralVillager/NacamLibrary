#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include "Object3d.h"
#include "IndirectObject3d.h"
#include <vector>
#include "Emitter.h"

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
