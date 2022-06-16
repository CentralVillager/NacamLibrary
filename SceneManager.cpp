#include "SceneManager.h"
#include "SplashScene.h"
#include "TitleScene.h"
#include "MainScene.h"
#include "TemporaryScene.h"
#include "ResultScene.h"
#include "DemoScene.h"
#include "GravityDemoScene.h"
#include "FrictionDemoScene.h"
#include "PostEffectDemoScene.h"
#include "ParticleDemoScene.h"
#include "RepulsionDemoScene.h"
#include "ImGuiManager.h"

stack<shared_ptr<AbstractScene>> SceneManager::scene_stack_;

void SceneManager::SetScene(Scene scene_name) {

	ClearSceneStack();

	// 入力されたシーンを生成し、スタックに追加
	if (scene_name == Scene::SPLASH) {

		scene_stack_.push(make_shared<SplashScene>());

	} else 	if (scene_name == Scene::TITLE) {

		scene_stack_.push(make_shared<TitleScene>());

	} else if (scene_name == Scene::MAIN) {

		scene_stack_.push(make_shared<MainScene>());

	} else if (scene_name == Scene::RESULT) {

		scene_stack_.push(make_shared<ResultScene>());

	} else if (scene_name == Scene::TEMPORARY) {

		scene_stack_.push(make_shared<TemporaryScene>());

	} else if (scene_name == Scene::DEMO) {

		scene_stack_.push(make_shared<DemoScene>());

	} else if (scene_name == Scene::GRAVITY_DEMO) {

		scene_stack_.push(make_shared<GravityDemoScene>());

	} else if (scene_name == Scene::FRICTION_DEMO) {

		scene_stack_.push(make_shared<FrictionDemoScene>());

	} else if (scene_name == Scene::POST_EFFECT_DEMO) {

		scene_stack_.push(make_shared<PostEffectDemoScene>());

	} else if (scene_name == Scene::PARTICLE_DEMO) {

		scene_stack_.push(make_shared<ParticleDemoScene>());

	} else if (scene_name == Scene::REPULSION_DEMO) {

		scene_stack_.push(make_shared<RepulsionDemoScene>());
	}

	// 追加したシーンの初期化
	scene_stack_.top()->Initialize();
}

void SceneManager::ReturnScene() {

	scene_stack_.top()->Finalize();

	scene_stack_.pop();
}

void SceneManager::ClearSceneStack() {

	while (!scene_stack_.empty()) {

		scene_stack_.top()->Finalize();

		scene_stack_.pop();
	}
}
