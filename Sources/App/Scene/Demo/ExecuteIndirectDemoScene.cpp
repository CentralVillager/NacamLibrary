#include <d3dcompiler.h>
#include "ExecuteIndirectDemoScene.h"
#include "../../../Lib/DebugUtill/NcmImGui.h"
#include "../../../Lib/DirectX/PreDraw.h"
#include "../../../Lib/Input/KeyboardInput.h"
#include "../../../Lib/DirectX/DirectXBase.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

ExecuteIndirectDemoScene::ExecuteIndirectDemoScene()
{
	// ÉJÉÅÉâÇÃê∂ê¨
	camera_ = make_unique<Camera>();

	indirect_obj_ = make_unique<IndirectObject3d>();
}

ExecuteIndirectDemoScene::~ExecuteIndirectDemoScene()
{}

void ExecuteIndirectDemoScene::Initialize()
{
	// ÉJÉÅÉâÇÃèâä˙âª
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 0, 0 });
	IndirectObject3d::SetCamera(camera_.get());

	indirect_obj_->Initialize();
}

void ExecuteIndirectDemoScene::Finalize()
{}

void ExecuteIndirectDemoScene::Update()
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{
		if (KeyboardInput::PushKey(DIK_W)) { camera_->MoveCameraTrack({ 0.0f, +1.0f, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_S)) { camera_->MoveCameraTrack({ 0.0f, -1.0f, 0.0f }); }
		if (KeyboardInput::PushKey(DIK_D)) { camera_->MoveCameraTrack({ +1.0f, 0.0f, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_A)) { camera_->MoveCameraTrack({ -1.0f, 0.0f, 0.0f }); }
		if (KeyboardInput::PushKey(DIK_R)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, +1.0f }); }
		else if (KeyboardInput::PushKey(DIK_F)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, -1.0f }); }
	}

	camera_->Update();

	if (KeyboardInput::TriggerKey(DIK_SPACE))
	{
		indirect_obj_->DetectPush(true);
	}
	else
	{
		indirect_obj_->DetectPush(false);
	}

	indirect_obj_->Update();
}

void ExecuteIndirectDemoScene::Draw()
{
	PreDraw::SetPipeline(PipelineName::IndirectObject3d_WireFrame);
	indirect_obj_->Draw();
}

void ExecuteIndirectDemoScene::DebugDraw()
{
	indirect_obj_->DebugDraw();
}
