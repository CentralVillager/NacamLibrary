#include "NacamLib.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ナカムライブラリのインスタンスを取得
	NacamLib *nacam_lib_ = NacamLib::GetInstance();

	/*-- 初期化処理 --*/
	//nacam_lib_->NacamLib_Initialize(Scene::EXECUTE_INDIRECT_DEMO);
	nacam_lib_->NacamLib_Initialize(Scene::PARTICLE_DEMO);

	/*-- ゲームループ --*/
	while (!nacam_lib_->CatchQuitSignal()) {

		/*-- 更新 --*/
		nacam_lib_->NacamLib_Update(60);

		/*-- 描画 --*/
		nacam_lib_->NacamLib_Draw();
	}

	/*-- 終了処理 --*/
	nacam_lib_->NacamLib_Finalize();

	return 0;
}
