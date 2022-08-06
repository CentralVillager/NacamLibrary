#include "Missile.h"

std::unique_ptr<Model> Missile::model_ = nullptr;

Missile::Missile() {

	if (!model_) {

		model_ = make_unique<Model>();
		model_->LoadObjModel("Resources/bullet/", "bullet.obj", "bullet.mtl");
	}

	object_ = make_unique<Object3d>();

	args_.pos = XMFLOAT3(0, 0, 0);
	args_.vel = XMFLOAT3(0, 0, 0);
	args_.acc = XMFLOAT3(0, 0, 0);
	args_.life = 0;
	args_.is_alive = false;
}

Missile::~Missile() {
}

void Missile::Initialize(const MissileArgs &args) {

	// ’l‚ð“ü—Í‚æ‚èÝ’è
	args_.pos = XMFLOAT3(args.pos);
	args_.vel = XMFLOAT3(args.vel);
	args_.acc = XMFLOAT3(args.acc);
	args_.life = args.life;
	args_.is_alive = args.is_alive;

	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetPosition(XMFLOAT3(args_.pos));
	object_->SetRotation(XMFLOAT3(0, 180.0f, 0));
}

void Missile::Finalize() {
}

void Missile::Update() {

	if (args_.life <= 0) {

		args_.is_alive = false;
		return;
	}

	MoveZ(1.0f);
	args_.life--;

	object_->Update();
}

void Missile::Draw() {

	object_->Draw();
}

void Missile::DebugDraw() {
}

void Missile::MoveZ(float speed) {

	XMFLOAT3 pos = object_->GetPosition();
	pos.x += args_.vel.x;
	pos.y += args_.vel.y;
	pos.z += args_.vel.z;
	object_->SetPosition(pos);
}

void Missile::FollowingTarget(XMFLOAT3 target_pos) {
}
