#include "Enemy.h"
#include "KeyboardInput.h"

std::unique_ptr<Model> Enemy::model_enemy_ = nullptr;
float Enemy::movable_range_[4] = {};
int Enemy::spawned_enemy_ = 0;
//std::unique_ptr<AudioManager> Enemy::death_ = nullptr;

void Enemy::InitializeModel() {

	// �G���f���̓ǂݍ���
	model_enemy_ = make_unique<Model>();
	model_enemy_->LoadObjModel("Resources/enemy/", "enemy.obj", "enemy.mtl");

	// �ړ��\�͈͂̐ݒ�
	movable_range_[(int)Direction::UP] = 14.0f + 5.0f;
	movable_range_[(int)Direction::DOWN] = -14.0f + 5.0f;
	movable_range_[(int)Direction::RIGHT] = 26.0f;
	movable_range_[(int)Direction::LEFT] = -26.0f;
}

void Enemy::Initialize() {

	death_ = make_shared<AudioManager>();
	death_->LoadWaveFile("Resources/Sounds/", "kill");

	// �G�I�u�W�F�N�g�̏�����
	enemy_ = make_shared<Object3d>();
	enemy_->Initialize();
	enemy_->SetModel(model_enemy_.get());
	enemy_->SetScale(0.5f);

	// �����蔻����Z�b�g
	collision.width = 3.5f;
	collision.height = 3.5f;
	collision.depth = 3.5f;
	collision.center = { enemy_->GetPosition() };
	collision.near_left_upper = {
		collision.center.x - collision.width / 2,
		collision.center.y + collision.height / 2,
		collision.center.z - collision.depth / 2
	};
	collision.near_right_downer = {
		collision.near_left_upper.x + collision.width,
		collision.near_left_upper.y - collision.height,
		collision.near_left_upper.z
	};
	collision.far_left_upper = {
		collision.near_left_upper.x,
		collision.near_left_upper.y,
		collision.near_left_upper.z + collision.depth
	};
	collision.far_right_downer = {
		collision.near_right_downer.x,
		collision.near_right_downer.y,
		collision.near_right_downer.z + collision.depth
	};
}

void Enemy::Finalize() {

	death_->UnloadWaveFile();
}

void Enemy::Update() {
	//Move();

	if (pattern_ == Pattern::HORIZONTAL) {

		MoveHorizontally();
	}
	if (pattern_ == Pattern::VERTICAL) {

		MoveVertically();
	}

	UpdateAABB();

	XMFLOAT3 rotation = enemy_->GetRotation();

	rotation.y += 1.0f;

	enemy_->SetRotation(rotation);

	// �|���ꂽ��
	if (!is_alive_) {

		death_->PlayWave(0.1f, AudioManager::SINGLE, false);

		// ���݂̈ʒu���擾
		XMFLOAT3 position = enemy_->GetPosition();

		position.y -= 3.0f;

		// �ύX�����ʒu�𔽉f
		enemy_->SetPosition(position);
	}

	// ������Ƃ���܂ŗ�������
	if (enemy_->GetPosition().y <= -20.0f) {

		is_alive_ = false;
	}

	enemy_->Update();
}

void Enemy::Draw() {
	enemy_->Draw();
}

void Enemy::Spawn(XMFLOAT3 pos, Pattern p) {

	Initialize();
	is_alive_ = true;
	move_speed_ = 0.3f;
	pattern_ = p;
	enemy_->SetPosition(pos);
}

void Enemy::Move() {

	// ���݂̈ʒu���擾
	XMFLOAT3 position = enemy_->GetPosition();

	// ��Ɉړ�
	if (KeyboardInput::PushKey(DIK_UP) && position.y < movable_range_[(int)Direction::UP]) { position.y += move_speed_; }
	//if (KeyboardInput::PushKey(DIK_UP)) { position.y += move_speed_; }

	// ���Ɉړ�
	if (KeyboardInput::PushKey(DIK_DOWN) && position.y > movable_range_[(int)Direction::DOWN]) { position.y -= move_speed_; }
	//if (KeyboardInput::PushKey(DIK_DOWN)) { position.y -= move_speed_; }

	// �E�Ɉړ�
	if (KeyboardInput::PushKey(DIK_RIGHT) && position.x < movable_range_[(int)Direction::RIGHT]) { position.x += move_speed_; }
	//if (KeyboardInput::PushKey(DIK_RIGHT)) { position.x += move_speed_; }

	// ���Ɉړ�
	if (KeyboardInput::PushKey(DIK_LEFT) && position.x > movable_range_[(int)Direction::LEFT]) { position.x -= move_speed_; }
	//if (KeyboardInput::PushKey(DIK_LEFT)) { position.x -= move_speed_; }

	if (!is_alive_) {
		position.y -= 3.0f;

	}

	// �ύX�����ʒu�𔽉f
	enemy_->SetPosition(position);
}

void Enemy::UpdateAABB() {

	collision.center = { enemy_->GetPosition() };

	collision.near_left_upper = {
		collision.center.x - collision.width / 2,
		collision.center.y + collision.height / 2,
		collision.center.z - collision.depth / 2
	};
	collision.near_right_downer = {
		collision.near_left_upper.x + collision.width,
		collision.near_left_upper.y - collision.height,
		collision.near_left_upper.z
	};
	collision.far_left_upper = {
		collision.near_left_upper.x,
		collision.near_left_upper.y,
		collision.near_left_upper.z + collision.depth
	};
	collision.far_right_downer = {
		collision.near_right_downer.x,
		collision.near_right_downer.y,
		collision.near_right_downer.z + collision.depth
	};
}

void Enemy::MoveHorizontally() {

	// ���݂̈ʒu���擾
	XMFLOAT3 position = enemy_->GetPosition();


	if (position.x >= movable_range_[(int)Direction::RIGHT] || position.x <= movable_range_[(int)Direction::LEFT]) {

		move_speed_ *= -1.0f;
	}

	position.x += move_speed_;

	// �ύX�����ʒu�𔽉f
	enemy_->SetPosition(position);
}

void Enemy::MoveVertically() {

	// ���݂̈ʒu���擾
	XMFLOAT3 position = enemy_->GetPosition();


	if (position.y >= movable_range_[(int)Direction::UP] || position.y <= movable_range_[(int)Direction::DOWN]) {

		move_speed_ *= -1.0f;
	}

	position.y += move_speed_;

	// �ύX�����ʒu�𔽉f
	enemy_->SetPosition(position);
}
