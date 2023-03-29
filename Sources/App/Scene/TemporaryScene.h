#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../Scene/AbsScene.h"
#include "../../Lib/Collision/CollisionPrimitive.h"
#include "../../Lib/Collision/Collision.h"
#include "../../Lib/DebugUtill/DebugText.h"
#include "../../Lib/Camera/Camera.h"
#include "../../Lib/3D/FbxLoader.h"
#include "../../Lib/3D/FbxObject3d.h"
#include "../../Lib/3D/Model.h"
#include "../../Lib/3D/Object3d.h"
#include "../../Lib/Audio/AudioManager.h"

class TemporaryScene : public AbsScene
{
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
