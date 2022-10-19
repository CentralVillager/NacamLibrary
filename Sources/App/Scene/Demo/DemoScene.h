#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../AbsScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/Fbx/FbxObject3d.h"
#include "../../../Lib/Fbx/FbxModel.h"

class DemoScene : public AbsScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;

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

	int texture_;
	int ease_;

	bool b = true;

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