#pragma once
#include <memory>
#include <wrl.h>
#include "../AbsScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/3D/Model.h"
#include "../../../Lib/3D/Object3d.h"
#include "../../../Lib/PostEffect/PostEffect.h"
#include "../../../Lib/2D/NcmSprite.h"

class PostEffectDemoScene : public AbsScene
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// モデル
	std::unique_ptr<Model> model_;

	// オブジェクト
	std::unique_ptr<Object3d> object_;

	// ポストエフェクト
	std::unique_ptr<PostEffect> post_effect_scene_;

	//std::unique_ptr<NcmSprite> back_;
	int back_;

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
