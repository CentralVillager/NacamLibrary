#include "SceneManager.h"
#include "../TitleScene.h"
#include "../MainScene.h"
#include "../ResultScene.h"
#include "../TemporaryScene.h"
#include "../Demo/DemoScene.h"
#include "../Demo/GravityDemoScene.h"
#include "../Demo/FrictionDemoScene.h"
#include "../Demo/PostEffectDemoScene.h"
#include "../Demo/ParticleDemoScene.h"
#include "../Demo/RepulsionDemoScene.h"
#include "../Demo/ExecuteIndirectDemoScene.h"
#include "../Demo/MathDemoScene.h"
#include "../../Debug/NcmImGui.h"
#include "../../../Lib/PostEffect/PostEffect.h"

stack<shared_ptr<AbsScene>> SceneManager::scene_stack_;
std::array<SceneName, (int)(SceneManager::SceneState::StateNum)> SceneManager::scene_state_;
float SceneManager::alpha_ = 1.0f;

bool SceneManager::InChangeScene(float speed)
{
	if (alpha_ >= 0.0f)
	{
		alpha_ -= speed;
		PostEffect::SetAplha(alpha_);
	}

	if (alpha_ <= 0.0f)
	{
		alpha_ = 0.0f;
		return true;
	}

	return false;
}

bool SceneManager::OutChangeScene(float speed)
{
	if (alpha_ <= 1.0f)
	{
		alpha_ += speed;
		PostEffect::SetAplha(alpha_);
	}

	if (alpha_ >= 1.0f)
	{
		alpha_ = 1.0f;
		return true;
	}

	return false;
}

void SceneManager::ExecuteSceneChange()
{
	using enum SceneState;

	// スタックを解放
	ClearSceneStack();

	scene_state_[(int)(Current)] = scene_state_[(int)(SceneState::Next)];

	// 入力されたシーンを生成し、スタックに追加
	if (scene_state_[(int)(Next)] == SceneName::SPLASH)
	{
		//scene_stack_.push(make_shared<QDSplashScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::TITLE)
	{
		scene_stack_.push(make_shared<TitleScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::MAIN)
	{
		scene_stack_.push(make_shared<MainScene>());
		//scene_stack_.push(make_shared<QDMainScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::RESULT)
	{
		scene_stack_.push(make_shared<ResultScene>());
		//scene_stack_.push(make_shared<QDResultScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::TEMPORARY)
	{
		scene_stack_.push(make_shared<TemporaryScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::DEMO)
	{
		scene_stack_.push(make_shared<DemoScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::GRAVITY_DEMO)
	{
		scene_stack_.push(make_shared<GravityDemoScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::FRICTION_DEMO)
	{
		scene_stack_.push(make_shared<FrictionDemoScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::POST_EFFECT_DEMO)
	{
		scene_stack_.push(make_shared<PostEffectDemoScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::PARTICLE_DEMO)
	{
		scene_stack_.push(make_shared<ParticleDemoScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::REPULSION_DEMO)
	{
		scene_stack_.push(make_shared<RepulsionDemoScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::EXECUTE_INDIRECT_DEMO)
	{
		scene_stack_.push(make_shared<ExecuteIndirectDemoScene>());
	}
	else if (scene_state_[(int)(Next)] == SceneName::MATH_DEMO)
	{
		scene_stack_.push(make_shared<MathDemoScene>());
	}

	// 追加したシーンの初期化
	scene_stack_.top()->Initialize();
}

void SceneManager::SetInitialScene(const SceneName &scene_name)
{
	scene_state_[(int)(SceneState::Next)] = scene_name;

	ExecuteSceneChange();
}

void SceneManager::SetNextScene(const SceneName &name)
{
	scene_state_[(int)(SceneState::Next)] = name;
}

bool SceneManager::NoticeChangeScene()
{
	using enum SceneState;

	if (scene_state_[(int)(Current)] != scene_state_[(int)(Next)])
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
	using enum SceneName;

	static int button;

	ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::RadioButton("Title", &button, 0))
	{
		SetNextScene(TITLE);
	}
	else if (ImGui::RadioButton("Main", &button, 1))
	{
		SetNextScene(MAIN);
	}
	else if (ImGui::RadioButton("Result", &button, 2))
	{
		SetNextScene(RESULT);
	}
	else if (ImGui::RadioButton("ParticleDemo", &button, 3))
	{
		SetNextScene(PARTICLE_DEMO);
	}
	else if (ImGui::RadioButton("Indirect", &button, 4))
	{
		SetNextScene(EXECUTE_INDIRECT_DEMO);
	}
	ImGui::End();
}
