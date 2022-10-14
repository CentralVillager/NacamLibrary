#include <sstream>
#include <iomanip>
#include "TemporaryScene.h"
#include "../Sources/App/Collision/Collision.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../../Lib/Fbx/FbxLoader.h"
#include "../../Lib/Input/KeyboardInput.h"
#include "../../Lib/Input/ControllerInput.h"

using namespace std;
using namespace DirectX;

TemporaryScene::TemporaryScene() {

	camera_ = make_unique<Camera>();
}

TemporaryScene::~TemporaryScene() {
}

void TemporaryScene::Initialize() {

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
}

void TemporaryScene::Finalize() {

}

void TemporaryScene::Update() {

	// カメラの移動
	if (KeyboardInput::PushKey(DIK_UP) || KeyboardInput::PushKey(DIK_DOWN) || KeyboardInput::PushKey(DIK_RIGHT) || KeyboardInput::PushKey(DIK_LEFT)) {
		if (KeyboardInput::PushKey(DIK_UP)) { camera_->MoveCameraTrack({ 0, 1.0f, 0 }); } else if (KeyboardInput::PushKey(DIK_DOWN)) { camera_->MoveCameraTrack({ 0, -1.0f, 0 }); }
		if (KeyboardInput::PushKey(DIK_RIGHT)) { camera_->MoveCameraTrack({ 1.0f, 0, 0 }); } else if (KeyboardInput::PushKey(DIK_LEFT)) { camera_->MoveCameraTrack({ -1.0f, 0, 0 }); }
	}

	// カメラの更新
	camera_->Update();
}

void TemporaryScene::Draw() {

}

void TemporaryScene::DebugDraw() {

}
