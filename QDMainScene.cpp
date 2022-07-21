#include "QDMainScene.h"
#include "Collision.h"
#include "FbxLoader.h"
#include "SceneManager.h"
#include "BackBoardManager.h"

using namespace std;
using namespace DirectX;

QDMainScene::QDMainScene() {

	// �J�����̐���
	camera_ = make_unique<Camera>();

	// �T�E���h�̏�����
	bgm_ = make_unique<AudioManager>();
	bgm_->LoadWaveFile("Resources/Sounds/", "main");

	// �v���C���[�̐���
	player_ = make_unique<Player>();
}

QDMainScene::~QDMainScene() {
}

void QDMainScene::Initialize() {

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	Object3d::SetCamera(camera_.get());
	PrimitiveObject::SetCamera(camera_.get());

	// �v���C���[�̏�����
	player_->Initialize(0.4f, 3.0f, 10);

	// �e�̃��f��������
	Bullet::InitializeModel();

	// �G�̃��f��������
	Enemy::InitializeModel();

	// �G���X�|�[��������
	SpawnEnemy({ 0, 0, 30.0f }, Pattern::NUTORAL);
	SpawnEnemy({ 10.0f, 0, 30.0f }, Pattern::NUTORAL);
	SpawnEnemy({ -10.0f, 0, 30.0f }, Pattern::NUTORAL);

	// �{�[�h�̏�����
	BackBoardManager::GetInstance()->Initialize();

	// �Q�[���I�[�o�[����𖳌��ɂ���
	is_game_over_ = false;
}

void QDMainScene::Finalize() {

	// �v���C���[�̏I������
	player_->Finalize();

	// �S�Ă̓G�ɑ΂���
	for (int i = 0; i < enemy_.size(); i++) {

		// �I������
		enemy_[i].Finalize();
	}

	// BGM���~�߂�
	bgm_->StopWave();

	// �A�����[�h����
	bgm_->UnloadWaveFile();
}

void QDMainScene::Update() {

	// ���E�F�[�u��
	static short wave = 0;

	// �J�����̍X�V
	camera_->Update();

	// �v���C���[�̍X�V
	player_->Update();

	// �G�l�~�[�̍X�V
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

	// �����蔻��
	for (int i = 0; i < player_->GetBulletVector().size(); i++) {

		for (int j = 0; j < enemy_.size(); j++) {

			if (Collision::CheckAABB2AABB(player_->GetBullet(i).GetAABB(), enemy_[j].GetAABB())) {

				player_->GetBullet(i).SetIsAlive(false);

				enemy_[j].SetIsAlive(false);

				Enemy::spawned_enemy_--;
			}
		}
	}

	// �����̍Đ�
	//main_->PlayWave(0.1f, AudioManager::LOOP, false);

	// �{�[�h�̍X�V
	BackBoardManager::GetInstance()->Update();

	// �Q�[���I�[�o�[���肪�L���Ȃ�
	if (is_game_over_) {

		// �I������
		Finalize();

		// ���U���g��
		SceneManager::SetScene(Scene::RESULT);
	}

	// �V�[�����ЂƂO�ɖ߂��i�����I�^�p�j
	if (KeyboardInput::TriggerKey(DIK_P)) {

		SceneManager::ReturnScene();
	}

	// �����I�Ƀ��U���g��
	if (KeyboardInput::TriggerKey(DIK_RETURN)) {

		Finalize();
		SceneManager::SetScene(Scene::RESULT);
	}
}

void QDMainScene::Draw() {

	Object3d::PreDraw();

	// �v���C���[�̕`��
	player_->Draw();

	// �{�[�h�̕`��
	BackBoardManager::GetInstance()->Draw();

	// �G�l�~�[�̕`��
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
