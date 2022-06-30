// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// エラー全部無視するやつ
//#pragma warning(disable : 4996)

#include "NacamLib.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ナカムライブラリのインスタンスを取得
	NacamLib *nacam_lib_ = NacamLib::GetInstance();

	/*-- 初期化処理 --*/
	nacam_lib_->NacamLib_Initialize(Scene::PARTICLE_DEMO);
	//nacam_lib_->NacamLib_Initialize(Scene::EXECUTE_INDIRECT_DEMO);

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



/*

	TODO
	//・ImGuiの表示制御
	//・GameSequenceを分離させ、SceneManagerクラスを作る
	・グリッドの表示
	・マウス入力
	・ライトの追加
	//・クアドラのリファクタリング
	//・DirectXBaseの整理
	・クォータニオンに対応
	・プリミティブオブジェクトの描画
	・XInputの実装
	・メモリリークの改善
	・今のところシーンがスタック積みである意味がない
	・DXCコンパイラの導入

	//・Releaseビルドが通らない！

*/