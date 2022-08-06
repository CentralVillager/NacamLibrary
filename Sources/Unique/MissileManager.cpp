#include "MissileManager.h"

MissileManager::MissileManager() {
}

MissileManager::~MissileManager() {
}

void MissileManager::Initialize() {

	for (auto &i : missile_list_) {

		//i.Initialize();
	}
}

void MissileManager::Finalize() {
}

void MissileManager::Update() {

	for (auto &i : missile_list_) {

		i.Update();
	}

	missile_list_.remove_if([](Missile &x) { return !x.GetIsAlive(); });
}

void MissileManager::Draw() {

	for (auto &i : missile_list_) {

		i.Draw();
	}
}

void MissileManager::DebugDraw() {
}

void MissileManager::Fire() {

	AddMissile();
}

void MissileManager::FollowingTarget(XMFLOAT3 target_pos) {
}

void MissileManager::AddMissile() {

	MissileArgs args{};
	args.pos = XMFLOAT3(0, 0, 0);
	args.vel = XMFLOAT3(0, 0, 1.0f);
	args.acc = XMFLOAT3(0, 0, 0);
	args.life = 100;
	args.is_alive = false;

	missile_list_.emplace_front();
	Missile &ptr = missile_list_.front();
	ptr.Initialize(args);
	ptr.Activate();
}
