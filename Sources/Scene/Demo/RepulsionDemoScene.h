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

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���f��
	std::unique_ptr<Model> model_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3d> ball1_;
	std::unique_ptr<Object3d> ball2_;

	// �����蔻��
	Sphere sphere1_;
	Sphere sphere2_;

	// ���x
	float velocity1_;
	float velocity2_;

	// ����
	float mass1_;
	float mass2_;

	// �^����
	float petus1_;
	float petus2_;

	bool is_hit_ = false;

	// �����W��
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
