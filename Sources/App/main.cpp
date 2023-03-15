#include "../Lib/NacamLib/NacamLib.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ナカムライブラリのインスタンスを取得
	NacamLib *nacam_lib_ = NacamLib::GetInstance();

	/*-- 初期化処理 --*/
	nacam_lib_->NacamLib_Initialize(SceneName::MAIN);

	/*-- ゲームループ --*/
	while (!nacam_lib_->CatchQuitSignal())
	{
		/*-- 更新 --*/
		nacam_lib_->NacamLib_Update();

		/*-- 描画 --*/
		nacam_lib_->NacamLib_Draw();
	}

	/*-- 終了処理 --*/
	nacam_lib_->NacamLib_Finalize();

	return 0;
}
