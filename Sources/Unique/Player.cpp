#include "Player.h"

Player::Player() {

	// モデルデータの生成
	model_ = std::make_shared<Model>();
	model_->LoadObjModel("Resources/low_fighter/", "new_low_fighter.obj", "new_low_fighter.mtl");

	object_ = std::make_shared<Object3d>();
}

Player::~Player() {
}

void Player::Initialize() {

	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetRotation({ 0, 180.0f, 0 });
}

void Player::Finalize() {
}

void Player::Update() {

	object_->Update();
}

void Player::Draw() {

	object_->Draw();
}

void Player::DebugDraw() {
}
