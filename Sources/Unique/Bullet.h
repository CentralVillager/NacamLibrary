#pragma once
#include <DirectXMath.h>
#include "Model.h"
#include "Object3d.h"
#include <memory>
#include "Collision.h"
#include "AudioManager.h"

class Bullet {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// �����蔻��
	AABB collision = {};

	// �e���f��
	static std::unique_ptr<Model> model_bullet_;

	// �e
	std::shared_ptr<Object3d> bullet_ = nullptr;

	// ���x
	float speed_;

	// ����
	int life_ = 0;

	// �������Ă��邩
	bool is_alive_ = false;

public:
	static void InitializeModel();

	void Initialize(float speed, int life);
	void Finalize();
	void Update();
	void Draw();

	const bool &GetIsAlive() { return is_alive_; }
	const AABB &GetAABB() { return collision; }
	void SetIsAlive(bool is_alive) { is_alive_ = is_alive; }

	/// <summary>
	/// �e�𐶐�����
	/// </summary>
	void Generate(XMFLOAT3 pos, float speed, int life);

	/// <summary>
	/// �e��O���ɔ�΂�
	/// </summary>
	void MoveForward();

	/// <summary>
	/// �����蔻��̈ʒu���X�V����
	/// </summary>
	void UpdateAABB();
};
