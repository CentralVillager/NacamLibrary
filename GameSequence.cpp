#include "GameSequence.h"
#include "SceneManager.h"
#include "FPSManager.h"

#include "DrawProc.h"

void GameSequence::MainGameLoop() {

	/*-- 初期化処理 --*/
	nacam_lib_->NacamLib_Initialize();

	// 通常止まらないエラーを強制的に止める
	nacam_lib_->DetectNonStopError(false);

	/*-- 初期シーンを設定 --*/
	//SceneManager::SetScene(Scene::REPULSION_DEMO);
	SceneManager::SetScene(Scene::PARTICLE_DEMO);

	/*-- ゲームループ --*/
	while (!nacam_lib_->CatchQuitSignal()) {

		/*-- fps制御 --*/
		FpsManager::RegulateFps(60);

		/*-- 更新 --*/
		nacam_lib_->NacamLib_Update();

		/*-- 描画 --*/
		nacam_lib_->NacamLib_Draw();

//#ifdef _DEBUG

		/*-- デバッグ描画 --*/
		nacam_lib_->NacamLib_DebugDraw();

//#endif

		/*-- 描画後処理 --*/
		nacam_lib_->NacamLib_PostDraw();
	}

	/*-- 終了処理 --*/
	nacam_lib_->NacamLib_Finalize();
}

// FBXシーンを解放するとエラーになる！
// ダーティーフラグを実装したい！
