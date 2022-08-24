#include "SceneManager.h"
#include "../TitleScene.h"
#include "../MainScene.h"
#include "../TemporaryScene.h"
#include "../Demo/DemoScene.h"
#include "../Demo/GravityDemoScene.h"
#include "../Demo/FrictionDemoScene.h"
#include "../Demo/PostEffectDemoScene.h"
#include "../Demo/ParticleDemoScene.h"
#include "../Demo/RepulsionDemoScene.h"
#include "../Demo/ExecuteIndirectDemoScene.h"
#include "../Demo/MathDemoScene.h"
#include "../../Debug/ImGuiManager.h"

stack<shared_ptr<AbstractScene>> SceneManager::scene_stack_;
std::array<SceneName, (int)(SceneManager::SceneState::StateNum)> SceneManager::scene_state_;

void SceneManager::ExecuteSceneChange()
{
	// スタックを解放
	ClearSceneStack();

	scene_state_[(int)(SceneState::Current)] = scene_state_[(int)(SceneState::Next)];

	// 入力されたシーンを生成し、スタックに追加
	if (scene_state_[(int)(SceneState::Next)] == SceneName::SPLASH)
	{
		//scene_stack_.push(make_shared<QDSplashScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::TITLE)
	{
		scene_stack_.push(make_shared<TitleScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::MAIN)
	{
		scene_stack_.push(make_shared<MainScene>());
		//scene_stack_.push(make_shared<QDMainScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::RESULT)
	{
		//scene_stack_.push(make_shared<QDResultScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::TEMPORARY)
	{
		scene_stack_.push(make_shared<TemporaryScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::DEMO)
	{
		scene_stack_.push(make_shared<DemoScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::GRAVITY_DEMO)
	{
		scene_stack_.push(make_shared<GravityDemoScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::FRICTION_DEMO)
	{
		scene_stack_.push(make_shared<FrictionDemoScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::POST_EFFECT_DEMO)
	{
		scene_stack_.push(make_shared<PostEffectDemoScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::PARTICLE_DEMO)
	{
		scene_stack_.push(make_shared<ParticleDemoScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::REPULSION_DEMO)
	{
		scene_stack_.push(make_shared<RepulsionDemoScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::EXECUTE_INDIRECT_DEMO)
	{
		scene_stack_.push(make_shared<ExecuteIndirectDemoScene>());
	}
	else if (scene_state_[(int)(SceneState::Next)] == SceneName::MATH_DEMO)
	{
		scene_stack_.push(make_shared<MathDemoScene>());
	}

	// 追加したシーンの初期化
	scene_stack_.top()->Initialize();
}

void SceneManager::SetInitialScene(SceneName scene_name)
{
	scene_state_[(int)(SceneState::Next)] = scene_name;

	ExecuteSceneChange();
}

void SceneManager::SetNextScene(SceneName name)
{
	scene_state_[(int)(SceneState::Next)] = name;
}

bool SceneManager::NoticeChangeScene()
{
	if (scene_state_[(int)(SceneState::Current)] != scene_state_[(int)(SceneState::Next)])
	{
		return true;
	}

	return false;
}

void SceneManager::ReturnScene()
{
	scene_stack_.top()->Finalize();
	scene_stack_.pop();
}

void SceneManager::ClearSceneStack()
{
	while (!scene_stack_.empty())
	{
		scene_stack_.top()->Finalize();
		scene_stack_.pop();
	}
}

void SceneManager::DebugDraw()
{
	static int button;

	ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::RadioButton("Title", &button, 0))
	{
		SetNextScene(SceneName::TITLE);
	}
	else if (ImGui::RadioButton("Main", &button, 1))
	{
		SetNextScene(SceneName::MAIN);
	}
	else if (ImGui::RadioButton("ParticleDemo", &button, 2))
	{
		SetNextScene(SceneName::PARTICLE_DEMO);
	}
	else if (ImGui::RadioButton("Indirect", &button, 3))
	{
		SetNextScene(SceneName::EXECUTE_INDIRECT_DEMO);
	}
	ImGui::End();
}
