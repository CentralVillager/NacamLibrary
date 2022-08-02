#pragma once
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../AbstractScene.h"
#include "../../Camera/Camera.h"
#include "../../Model/Model.h"
#include "../../Object3d/Object3d.h"
#include "../../Collision/CollisionPrimitive.h"

class RepulsionDemoScene : public AbstractScene {
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
	void DebugDraw() override;

};
