#pragma once
#include "InputManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "Singleton.h"
#include "NacamLib.h"
#include <memory>

#include "PostEffectDemoScene.h"
#include "PostEffect.h"
#include "DirectXBase.h"

class GameSequence final : public Singleton<GameSequence> {
	friend Singleton<GameSequence>;

	NacamLib *nacam_lib_ = NacamLib::GetInstance();

public:

	/// <summary>
	/// ここで全てのUpdateとDrawを行います。
	/// </summary>
	void MainGameLoop();
};

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

	//・Releaseビルドが通らない！

*/
