#pragma once
#include <memory>
#include <wrl.h>
#include "../AbsScene.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/Model/Model.h"
#include "../../../Lib/Object3d/Object3d.h"

class GravityDemoScene : public AbsScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// モデル
	std::unique_ptr<Model> model_;

	// オブジェクト
	std::unique_ptr<Object3d> object_;

private:

	// 重力加速度
	const float G_ACCELERATION_ = 9.8f;

private:

	// 位置
	float pos_ = 0;

	// 加速度
	float g_ = G_ACCELERATION_ / 60.0f;

	// 速度
	float velocity_ = 0;

	// カスタム加速度
	float acceleration_ = 0.1f;

private:

	int frame_count_ = 0;

public:

	GravityDemoScene();
	~GravityDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;

public:

	/// <summary>
	/// 落下
	/// </summary>
	void Fall();

	/// <summary>
	/// 待機するかどうか
	/// </summary>
	bool Wait();
};
