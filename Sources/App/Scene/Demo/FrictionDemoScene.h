#pragma once
#include <memory>
#include <wrl.h>
#include "../AbsScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/3D/Model.h"
#include "../../../Lib/3D/Object3d.h"

class FrictionDemoScene : public AbsScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// モデル
	std::unique_ptr<Model> model_;

	// オブジェクト
	std::unique_ptr<Object3d> object_;

public:

	FrictionDemoScene();
	~FrictionDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};

