#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../Scene/AbstractScene.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Collision/Collision.h"
#include "../Debug/DebugText.h"
#include "../Camera/Camera.h"
#include "../Fbx/FbxLoader.h"
#include "../Fbx/FbxObject3d.h"
#include "../Model/Model.h"
#include "../Object3d/Object3d.h"
#include "../Audio/AudioManager.h"

class TemporaryScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// ÉJÉÅÉâ
	std::unique_ptr<Camera> camera_;

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
