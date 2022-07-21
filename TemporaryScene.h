#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "AbstractScene.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "DebugText.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "PrimitiveObject.h"
#include "Model.h"
#include "Object3d.h"
#include "AudioManager.h"

class TemporaryScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// グリッド
	std::unique_ptr<PrimitiveObject> line_;

public:

	TemporaryScene();
	~TemporaryScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
