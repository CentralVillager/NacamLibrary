#pragma once
#include <memory>
#include <wrl.h>
#include "../AbstractScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/Fbx/FbxObject3d.h"
#include "../../../Lib/Fbx/FbxModel.h"

class DemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// FBXオブジェクト
	//std::unique_ptr<FbxObject3d> fbx_obj_;
	FbxObject3d *fbx_obj_;

	//// FBXモデル
	//std::unique_ptr<FbxModel> fbx_model_;
	FbxModel *fbx_model_;


public:

	DemoScene();
	~DemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
