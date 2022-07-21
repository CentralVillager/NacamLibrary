#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include "Model.h"
#include "Object3d.h"
#include <memory>
#include <wrl.h>

class FrictionDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���f��
	std::unique_ptr<Model> model_;

	// �I�u�W�F�N�g
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

