#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Model.h"
#include "Object3d.h"
#include "Bullet.h"
#include <vector>
#include "Collision.h"

class Player {
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

	// �v���C���[���f��
	std::shared_ptr<Model> model_fighter_ = nullptr;

	// �v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_ = nullptr;

	// �X�s�[�h
	float move_speed_ = 0;

	// �e��
	float bullet_speed_ = 0;

	// �e�̘A�ˑ��x
	int rapid_fire_speed_ = 0;

	// �e�̎���
	int bullet_life_ = 0;

	// HP
	std::unique_ptr<uint16_t> hit_point_ = 0;

	// �ړ��\�͈�
	float movable_range_[4] = {};

	// �e
	std::vector<Bullet> bullet_ = {};

	std::unique_ptr<AudioManager> shot_;

public:

	Player() {};
	~Player() {};

	const vector<Bullet> &GetBulletVector() { return bullet_; }
	Bullet &GetBullet(int num) { return bullet_[num]; }
	const AABB &GetAABB() { return collision; }

	void Initialize(float speed, float bullet_speed, int rapid_fire_speed);
	void Finalize();
	void Update();
	void Draw();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �e�̔���
	/// </summary>
	void Shot();

	/// <summary>
	/// �������s�����e��vector�z�񂩂�폜����
	/// </summary>
	void DeleteDeadBullet();

	/// <summary>
	/// �����蔻��̈ʒu���X�V����
	/// </summary>
	void UpdateAABB();
};
