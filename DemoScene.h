#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include <memory>
#include <wrl.h>

class DemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// �J����
	std::unique_ptr<Camera> camera_;

	// FBX�I�u�W�F�N�g
	//std::unique_ptr<FbxObject3d> fbx_obj_;
	FbxObject3d *fbx_obj_;

	//// FBX���f��
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
