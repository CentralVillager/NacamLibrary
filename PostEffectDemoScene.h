#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include "Model.h"
#include "Object3d.h"
#include <memory>
#include <wrl.h>
#include "PostEffect.h"
#include "Sprite.h"

class PostEffectDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCmdList().Get();

private:

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���f��
	std::unique_ptr<Model> model_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3d> object_;

	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> post_effect_scene_;

	std::unique_ptr<Sprite> back_;

	bool is_push_;

public:

	PostEffectDemoScene();
	~PostEffectDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;

private:

	void RollY();

};
