#pragma once
#include <stack>
#include <memory>
#include "AbstractScene.h"
#include "Singleton.h"

// 全シーン名
enum class Scene {

	SPLASH,		// ロゴなどを表示
	TITLE,		// タイトルを表示
	MAIN,		// ゲームのメイン要素のシーン
	RESULT,		// リザルトを表示
	TEMPORARY,	// デバッグや検証などの仮シーン
	DEMO,		// 何かを見せる用のシーン
	GRAVITY_DEMO,
	FRICTION_DEMO,
	POST_EFFECT_DEMO,
	PARTICLE_DEMO,
	REPULSION_DEMO,

};

class SceneManager : public Singleton<SceneManager> {
	friend Singleton<SceneManager>;

	// シーンを溜めるためのスタック
	static stack<shared_ptr<AbstractScene>> scene_stack_;

public:

	static const stack<shared_ptr<AbstractScene>> &GetSceneStack() { return scene_stack_; }

	/// <summary>
	/// 指定したシーンに移ります。
	/// </summary>
	/// <param name="scene_name">次に映したいシーン名</param>
	static void SetScene(Scene scene_name);

	/// <summary>
	/// ひとつ前のシーンに戻ります。
	/// </summary>
	static void ReturnScene();

	/// <summary>
	/// スタックに積まれているシーンを全て解除します。
	/// </summary>
	static void ClearSceneStack();
};
