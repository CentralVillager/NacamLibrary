#include "Player.h"
#include "KeyboardInput.h"
#include "ImGuiManager.h"
#include "ControllerInput.h"

using namespace DirectX;

void Player::Initialize(float speed, float bullet_speed, int rapid_fire_speed) {
	shot_ = make_unique<AudioManager>();
	shot_->LoadWaveFile("Resources/Sounds/", "shot");

	// ���f���f�[�^�̓ǂݍ���
	model_fighter_ = make_shared<Model>();
	model_fighter_->LoadObjModel("Resources/low_fighter/", "low_fighter.obj", "low_fighter.mtl");

	// �v���C���[�I�u�W�F�N�g�̏�����
	player_ = make_unique<Object3d>();
	player_->Initialize();
	player_->SetModel(model_fighter_.get());
	player_->SetRotation({ 0, 180.0f, 0 });
	player_->SetPosition({ 0, 0, 10.0f });

	// �����蔻����Z�b�g
	collision.width = 2.0f;
	collision.height = 1.0f;
	collision.depth = 3.0f;
	collision.center = { player_->GetPosition() };
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

	// �X�s�[�h���Z�b�g
	move_speed_ = speed;

	// �e�����Z�b�g
	bullet_speed_ = bullet_speed;

	// �A�ˑ��x���Z�b�g
	rapid_fire_speed_ = rapid_fire_speed;

	bullet_life_ = 100;

	// �ړ��\�͈͂̐ݒ�
	movable_range_[(int)Direction::UP] = 14.0f + 5.0f;
	movable_range_[(int)Direction::DOWN] = -14.0f + 5.0f;
	movable_range_[(int)Direction::RIGHT] = 26.0f;
	movable_range_[(int)Direction::LEFT] = -26.0f;
}

void Player::Finalize() {
	shot_->UnloadWaveFile();
}

void Player::Update() {
	static int cool_time = 0;

	cool_time++;

	// ����
	if (KeyboardInput::PushKey(DIK_SPACE) && rapid_fire_speed_ <= cool_time ||
		ControllerInput::PushButton(DIB_R) && rapid_fire_speed_ <= cool_time) {

		Shot();

		shot_->PlayWave(0.1f, AudioManager::SINGLE, true);

		cool_time = 0;
	}

	// �v���C���[�̈ړ�
	if (KeyboardInput::PushKey(DIK_W) ||
		KeyboardInput::PushKey(DIK_A) ||
		KeyboardInput::PushKey(DIK_S) ||
		KeyboardInput::PushKey(DIK_D)) {

		Move();
	} else if (ControllerInput::LStickUp() ||
			   ControllerInput::LStickDown() ||
			   ControllerInput::LStickLeft() ||
			   ControllerInput::LStickRight()) {

		Move();
	}

	/// �������s�����e��vector�z�񂩂�폜����
	DeleteDeadBullet();

	// �����蔻��̈ʒu���X�V����
	UpdateAABB();

	player_->Update();

	for (int i = 0; i < bullet_.size(); i++) {

		bullet_[i].Update();
	}
}

void Player::Draw() {
	// ImGui�p�̈ꎞ�ϐ�
	float position[3] = {
		player_->GetPosition().x,
		player_->GetPosition().y,
		player_->GetPosition().z
	};

	// ImGui�`��
	/*ImGui::Text("Player Debug");
	ImGui::SliderFloat3("player_position", position, -20.0f, 20.0f);
	ImGui::SliderFloat("player_speed", &move_speed_, 0, 3.0f);
	ImGui::SliderFloat("bullet_speed", &bullet_speed_, 0, 50.0f);
	ImGui::SliderInt("rapid_fire_speed", &rapid_fire_speed_, 0, 100);
	ImGui::SliderInt("bullet_life", &bullet_life_, 0, 500);*/

	// �ꎞ�ϐ��𔽉f
	player_->SetPosition({ position[0], position[1], position[2] });

	// �`��
	player_->Draw();

	// �e�̕`��
	for (int i = 0; i < bullet_.size(); i++) {

		bullet_[i].Draw();
	}
}

void Player::Move() {

	// ���݂̈ʒu���擾
	XMFLOAT3 position = player_->GetPosition();

	// ��Ɉړ�
	if (KeyboardInput::PushKey(DIK_W) && position.y < movable_range_[(int)Direction::UP]) { position.y += move_speed_; } else if (ControllerInput::LStickUp() && position.y < movable_range_[(int)Direction::UP]) { position.y += move_speed_; }

	// ���Ɉړ�
	if (KeyboardInput::PushKey(DIK_S) && position.y > movable_range_[(int)Direction::DOWN]) { position.y -= move_speed_; } else if (ControllerInput::LStickDown() && position.y > movable_range_[(int)Direction::DOWN]) { position.y -= move_speed_; }

	// �E�Ɉړ�
	if (KeyboardInput::PushKey(DIK_D) && position.x < movable_range_[(int)Direction::RIGHT]) { position.x += move_speed_; } else if (ControllerInput::LStickRight() && position.x < movable_range_[(int)Direction::RIGHT]) { position.x += move_speed_; }

	// ���Ɉړ�
	if (KeyboardInput::PushKey(DIK_A) && position.x > movable_range_[(int)Direction::LEFT]) { position.x -= move_speed_; } else if (ControllerInput::LStickLeft() && position.x > movable_range_[(int)Direction::LEFT]) { position.x -= move_speed_; }

	// �ύX�����ʒu�𔽉f
	player_->SetPosition(position);
}

void Player::Shot() {

	// �V���Ȓe�𐶐�
	Bullet bullet;

	// �e�𐶐�
	bullet.Generate(player_->GetPosition(), bullet_speed_, bullet_life_);

	// �z��ɓo�^
	bullet_.push_back(bullet);
}

void Player::DeleteDeadBullet() {

	// �S�Ă̗L���Ȓe�ɑ΂���
	for (int i = 0; i < bullet_.size(); i++) {

		// �������s���Ă�����
		if (!bullet_[i].GetIsAlive()) {

			// �z�񂩂�폜����
			bullet_.erase(bullet_.begin() + i);
		}
	}
}

void Player::UpdateAABB() {

	collision.center = { player_->GetPosition() };
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
