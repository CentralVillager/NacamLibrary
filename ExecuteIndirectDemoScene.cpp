#include "ExecuteIndirectDemoScene.h"
#include "DirectXBase.h"
#include <d3dcompiler.h>
#include "KeyboardInput.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

ExecuteIndirectDemoScene::ExecuteIndirectDemoScene() {

	// ÉJÉÅÉâÇÃê∂ê¨
	camera_ = make_unique<Camera>();

	indirect_obj_ = make_unique<IndirectObject3d>();
}

ExecuteIndirectDemoScene::~ExecuteIndirectDemoScene() {
}

void ExecuteIndirectDemoScene::Initialize() {

	// ÉJÉÅÉâÇÃèâä˙âª
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 0, 0 });
	IndirectObject3d::SetCamera(camera_.get());

	indirect_obj_->Initialize();
}

void ExecuteIndirectDemoScene::Finalize() {
}

void ExecuteIndirectDemoScene::Update() {

	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F)) {

		if (KeyboardInput::PushKey(DIK_W)) { camera_->MoveCameraTrack({ 0.0f,+1.0f,0.0f }); } else if (KeyboardInput::PushKey(DIK_S)) { camera_->MoveCameraTrack({ 0.0f,-1.0f,0.0f }); }
		if (KeyboardInput::PushKey(DIK_D)) { camera_->MoveCameraTrack({ +1.0f, 0.0f, 0.0f }); } else if (KeyboardInput::PushKey(DIK_A)) { camera_->MoveCameraTrack({ -1.0f,0.0f,0.0f }); }
		if (KeyboardInput::PushKey(DIK_R)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, +1.0f }); } else if (KeyboardInput::PushKey(DIK_F)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, -1.0f }); }
	}

	camera_->Update();

	indirect_obj_->Update();
}

void ExecuteIndirectDemoScene::Draw() {

	IndirectObject3d::PreDraw();
	indirect_obj_->Draw();
}

void ExecuteIndirectDemoScene::DebugDraw() {
}
