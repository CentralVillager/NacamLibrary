#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../AbsScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/Fbx/FbxObject3d.h"
#include "../../../Lib/Fbx/FbxModel.h"
#include "../../../Lib/Point/Point.h"
#include "../../../Lib/PlatePoly/PlatePoly.h"
#include "../../Grid/GridRender.h"

class DemoScene : public AbsScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// FBXオブジェクト
	//std::unique_ptr<FbxObject3d> fbx_obj_;
	//FbxObject3d *fbx_obj_;

	//// FBXモデル
	//std::unique_ptr<FbxModel> fbx_model_;
	//FbxModel *fbx_model_;s

	int texture_;
	int ease_;
	int test_;

	bool b = true;

	std::unique_ptr<GridRender> grid_;
	std::unique_ptr<GridRender> ugrid_;

	std::unique_ptr<Point> point_;
	std::unique_ptr<PlatePoly> poly_;

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
