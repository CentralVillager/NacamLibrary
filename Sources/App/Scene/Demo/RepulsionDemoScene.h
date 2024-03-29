#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../AbsScene.h"
#include "../../../Lib/Collision/CollisionPrimitive.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/3D/Model.h"
#include "../../../Lib/3D/Object3d.h"

class RepulsionDemoScene : public AbsScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// モデル
	std::unique_ptr<Model> model_;

	// オブジェクト
	std::unique_ptr<Object3d> ball1_;
	std::unique_ptr<Object3d> ball2_;

	// 当たり判定
	Sphere sphere1_;
	Sphere sphere2_;

	// 速度
	float velocity1_;
	float velocity2_;

	// 質量
	float mass1_;
	float mass2_;

	// 運動量
	float petus1_;
	float petus2_;

	bool is_hit_ = false;

	// 反発係数
	float e_;

public:

	RepulsionDemoScene();
	~RepulsionDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void AfterPostEffectDraw() override;
	void DebugDraw() override;

};
