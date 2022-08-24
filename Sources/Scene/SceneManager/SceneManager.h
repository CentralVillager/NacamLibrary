#pragma once
#include <stack>
#include <memory>
#include <array>
#include "../AbstractScene.h"

// 全シーン名
enum class SceneName : unsigned int
{
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
	EXECUTE_INDIRECT_DEMO,
	MATH_DEMO
};

class SceneManager
{
	enum class SceneState : unsigned int
	{
		Current,
		Next,
		StateNum
	};

	// シーンを溜めるためのスタック
	static stack<shared_ptr<AbstractScene>> scene_stack_;

	static std::array<SceneName, (int)(SceneState::StateNum)> scene_state_;

public:

	static const stack<shared_ptr<AbstractScene>> &GetSceneStack() { return scene_stack_; }

	/// <summary>
	/// シーン切り替えの実行
	/// </summary>
	static void ExecuteSceneChange();

	/// <summary>
	/// アプリ初期化時のシーンの設定
	/// </summary>
	/// <param name="scene_name"></param>
	static void SetInitialScene(SceneName scene_name);

	/// <summary>
	/// 次に映したいシーンのセット
	/// アプリ内でのシーン切り替え時には基本的にこの関数を使用
	/// </summary>
	static void SetNextScene(SceneName name);

	/// <summary>
	/// シーン切り替えが行われようとしていることを通知
	/// </summary>
	static bool NoticeChangeScene();

	/// <summary>
	/// ひとつ前のシーンに戻る
	/// </summary>
	static void ReturnScene();

	/// <summary>
	/// スタックに積まれているシーンを全て解除
	/// </summary>
	static void ClearSceneStack();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	static void DebugDraw();
};
