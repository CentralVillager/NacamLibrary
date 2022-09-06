#include "ResultScene.h"
#include "../Input/KeyboardInput.h"
#include "../Scene/SceneManager/SceneManager.h"

ResultScene::ResultScene()
{
	result_ = std::make_unique<Sprite>();
}

ResultScene::~ResultScene()
{}

void ResultScene::Initialize()
{

}

void ResultScene::Finalize()
{}

void ResultScene::Update()
{
	if (KeyboardInput::TriggerKey(DIK_SPACE))
	{
		SceneManager::SetNextScene(SceneName::TITLE);
	}
}

void ResultScene::Draw()
{}

void ResultScene::DebugDraw()
{}