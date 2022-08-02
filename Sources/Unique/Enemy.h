#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Model.h"
#include "Object3d.h"
#include "Collision.h"
#include "AudioManager.h"

enum class Pattern {
	NUTORAL,
	HORIZONTAL,
	VERTICAL
};

class Enemy {
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	// ����
	enum class Direction {
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

private:

	// �����蔻��
	AABB collision = {};

	// �G���f��
	static std::unique_ptr<Model> model_enemy_;

	// �G�I�u�W�F�N�g
	std::shared_ptr<Object3d> enemy_ = nullptr;

	// �X�s�[�h
	float move_speed_ = 0;

	// �ړ��\�͈�
	static float movable_range_[4];

	// �������Ă��邩
	bool is_alive_ = false;

	std::shared_ptr<AudioManager> death_;

	Pattern pattern_;

public:

	// ���ݏo�����Ă���G�̐�
	static int spawned_enemy_;

	static void InitializeModel();

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	const AABB &GetAABB() { return collision; }
	void SetIsAlive(bool is_alive) { is_alive_ = is_alive; }

	void Spawn(XMFLOAT3 pos, Pattern p);

	void Move();

	// ���E�ɔ����ړ�
	void MoveHorizontally();

	// �㉺�ɔ����ړ�
	void MoveVertically();

	/// <summary>
	/// �����蔻��̈ʒu���X�V����
	/// </summary>
	void UpdateAABB();
};
