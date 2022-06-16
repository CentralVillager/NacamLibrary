#include "TemporaryScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include "FbxLoader.h"
#include "SceneManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"

using namespace std;
using namespace DirectX;

TemporaryScene::TemporaryScene() {

	camera_ = make_unique<Camera>();
	line_ = make_unique<PrimitiveObject>();
}

TemporaryScene::~TemporaryScene() {
}

void TemporaryScene::Initialize() {

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);

	// �O���b�h�̏�����
	line_->Initialize({ -0.5f, 0, 0 }, { 0.5f, 0, 0 });
	PrimitiveObject::SetCamera(camera_.get());
}

void TemporaryScene::Finalize() {

}

void TemporaryScene::Update() {

	// �J�����̈ړ�
	if (KeyboardInput::PushKey(DIK_UP) || KeyboardInput::PushKey(DIK_DOWN) || KeyboardInput::PushKey(DIK_RIGHT) || KeyboardInput::PushKey(DIK_LEFT)) {
		if (KeyboardInput::PushKey(DIK_UP)) { camera_->MoveCameraTrack({ 0, 1.0f, 0 }); } else if (KeyboardInput::PushKey(DIK_DOWN)) { camera_->MoveCameraTrack({ 0, -1.0f, 0 }); }
		if (KeyboardInput::PushKey(DIK_RIGHT)) { camera_->MoveCameraTrack({ 1.0f, 0, 0 }); } else if (KeyboardInput::PushKey(DIK_LEFT)) { camera_->MoveCameraTrack({ -1.0f, 0, 0 }); }
	}

	// �O���b�h�̍X�V
	line_->Update();

	// �J�����̍X�V
	camera_->Update();
}

void TemporaryScene::Draw() {

	PrimitiveObject::PreDraw();
	line_->Draw();
}

void TemporaryScene::DebugDraw() {

	XMFLOAT3 pos = line_->GetPosition();

	ImGui::Text("Grid");
	ImGui::Text("s_pos = { %f, %f, %f }", pos.x, pos.y, pos.z);
}
