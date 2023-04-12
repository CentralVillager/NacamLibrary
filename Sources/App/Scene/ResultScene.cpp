#include "ResultScene.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../../Lib/Input/KeyboardInput.h"

ResultScene::ResultScene()
{
	//result_ = std::make_unique<NcmSprite>();
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

void ResultScene::AfterPostEffectDraw()
{}

void ResultScene::DebugDraw()
{}
