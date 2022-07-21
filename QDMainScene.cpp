#include "QDMainScene.h"
#include "Collision.h"
#include "FbxLoader.h"
#include "SceneManager.h"
#include "BackBoardManager.h"

using namespace std;
using namespace DirectX;

QDMainScene::QDMainScene() {

	// カメラの生成
	camera_ = make_unique<Camera>();

	// サウンドの初期化
	bgm_ = make_unique<AudioManager>();
	bgm_->LoadWaveFile("Resources/Sounds/", "main");

	// プレイヤーの生成
	player_ = make_unique<Player>();
}

QDMainScene::~QDMainScene() {
}

void QDMainScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());
	PrimitiveObject::SetCamera(camera_.get());

	// プレイヤーの初期化
	player_->Initialize(0.4f, 3.0f, 10);

	// 弾のモデル初期化
	Bullet::InitializeModel();

	// 敵のモデル初期化
	Enemy::InitializeModel();

	// 敵をスポーンさせる
	SpawnEnemy({ 0, 0, 30.0f }, Pattern::NUTORAL);
	SpawnEnemy({ 10.0f, 0, 30.0f }, Pattern::NUTORAL);
	SpawnEnemy({ -10.0f, 0, 30.0f }, Pattern::NUTORAL);

	// ボードの初期化
	BackBoardManager::GetInstance()->Initialize();

	// ゲームオーバー判定を無効にする
	is_game_over_ = false;
}

void QDMainScene::Finalize() {

	// プレイヤーの終了処理
	player_->Finalize();

	// 全ての敵に対して
	for (int i = 0; i < enemy_.size(); i++) {

		// 終了処理
		enemy_[i].Finalize();
	}

	// BGMを止める
	bgm_->StopWave();

	// アンロードする
	bgm_->UnloadWaveFile();
}

void QDMainScene::Update() {

	// 現ウェーブ数
	static short wave = 0;

	// カメラの更新
	camera_->Update();

	// プレイヤーの更新
	player_->Update();

	// エネミーの更新
	for (int i = 0; i < enemy_.size(); i++) {
		enemy_[i].Update();
	}

	if (wave == 0 && Enemy::spawned_enemy_ <= 0) {

		SpawnEnemy({ 0, 0, 30.0f }, Pattern::NUTORAL);
		SpawnEnemy({ 0, 10.0f, 30.0f }, Pattern::NUTORAL);
		SpawnEnemy({ 0, -10.0f, 30.0f }, Pattern::NUTORAL);

		wave = 1;
	}

	if (wave == 1 && Enemy::spawned_enemy_ <= 0) {

		SpawnEnemy({ 0, 10.0f, 30.0f }, Pattern::HORIZONTAL);
		SpawnEnemy({ 15.0f, 0, 30.0f }, Pattern::HORIZONTAL);
		SpawnEnemy({ -15.0f, -10.0f, 30.0f }, Pattern::HORIZONTAL);

		wave = 2;
	}

	if (wave == 2 && Enemy::spawned_enemy_ <= 0) {

		SpawnEnemy({ 0, 10.0f, 30.0f }, Pattern::VERTICAL);
		SpawnEnemy({ 15.0f, 0, 30.0f }, Pattern::VERTICAL);
		SpawnEnemy({ -15.0f, -8.0f, 30.0f }, Pattern::VERTICAL);

		wave = 3;
	}

	if (wave == 3 && Enemy::spawned_enemy_ <= 0) {

		is_game_over_ = true;

		wave = 0;
	}

	// 当たり判定
	for (int i = 0; i < player_->GetBulletVector().size(); i++) {

		for (int j = 0; j < enemy_.size(); j++) {

			if (Collision::CheckAABB2AABB(player_->GetBullet(i).GetAABB(), enemy_[j].GetAABB())) {

				player_->GetBullet(i).SetIsAlive(false);

				enemy_[j].SetIsAlive(false);

				Enemy::spawned_enemy_--;
			}
		}
	}

	// 音声の再生
	//main_->PlayWave(0.1f, AudioManager::LOOP, false);

	// ボードの更新
	BackBoardManager::GetInstance()->Update();

	// ゲームオーバー判定が有効なら
	if (is_game_over_) {

		// 終了処理
		Finalize();

		// リザルトへ
		SceneManager::SetScene(Scene::RESULT);
	}

	// シーンをひとつ前に戻す（試験的運用）
	if (KeyboardInput::TriggerKey(DIK_P)) {

		SceneManager::ReturnScene();
	}

	// 強制的にリザルトへ
	if (KeyboardInput::TriggerKey(DIK_RETURN)) {

		Finalize();
		SceneManager::SetScene(Scene::RESULT);
	}
}

void QDMainScene::Draw() {

	Object3d::PreDraw();

	// プレイヤーの描画
	player_->Draw();

	// ボードの描画
	BackBoardManager::GetInstance()->Draw();

	// エネミーの描画
	for (int i = 0; i < enemy_.size(); i++) {
		enemy_[i].Draw();
	}
}

void QDMainScene::DebugDraw() {

	ImGui::Text("AABB Enemy");
	ImGui::Text("left_upper = { %f, %f, %f }", enemy_[0].GetAABB().near_left_upper.x, enemy_[0].GetAABB().near_left_upper.y, enemy_[0].GetAABB().near_left_upper.z);
	ImGui::Text("right_downer = { %f, %f, %f }", enemy_[0].GetAABB().near_right_downer.x, enemy_[0].GetAABB().near_right_downer.y, enemy_[0].GetAABB().near_right_downer.z);
}

void QDMainScene::SpawnEnemy(XMFLOAT3 pos, Pattern p) {

	Enemy enemy;
	enemy.Spawn(pos, p);
	enemy_.push_back(enemy);

	Enemy::spawned_enemy_++;
}
