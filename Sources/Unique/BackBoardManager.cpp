#include "BackBoardManager.h"

BackBoardManager::BackBoardManager() {
}

BackBoardManager::~BackBoardManager() {
}

void BackBoardManager::Initialize() {

	// 起動時用に初期化
	back_board_[0].Initialize({ 0, -30.0f, 100.0f }, true);
	back_board_[1].Initialize({ 0, -30.0f, 200.0f }, true);
	back_board_[2].Initialize({ 0, -30.0f, 300.0f }, true);
	back_board_[3].Initialize({ 0, -30.0f, 400.0f }, true);
	back_board_[4].Initialize({ 0, -30.0f, 500.0f }, true);
	back_board_[5].Initialize({ 0, 30.0f, 100.0f }, false);
	back_board_[6].Initialize({ 0, 30.0f, 200.0f }, false);
	back_board_[7].Initialize({ 0, 30.0f, 300.0f }, false);
	back_board_[8].Initialize({ 0, 30.0f, 400.0f }, false);
	back_board_[9].Initialize({ 0, 30.0f, 500.0f }, false);
}

void BackBoardManager::Finalize() {
}

void BackBoardManager::Update() {

	// 全て
	for (int i = 0; i < back_board_.size(); i++) {

		// 更新
		back_board_[i].Update();
	}
}

void BackBoardManager::Draw() {

	// 全て
	for (int i = 0; i < back_board_.size(); i++) {

		// 描画
		back_board_[i].Draw();
	}
}

void BackBoardManager::DebugDraw() {
}
