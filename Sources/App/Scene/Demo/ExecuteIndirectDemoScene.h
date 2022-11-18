#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>
#include "../AbsScene.h"
#include "../../Particle/Emitter.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/Model/Model.h"
#include "../../../Lib/Object3d/Object3d.h"
#include "../../../Lib/IndirectObject3d/IndirectObject3d.h"

class ExecuteIndirectDemoScene : public AbsScene
{
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
