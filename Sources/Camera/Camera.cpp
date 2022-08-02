#include "Camera.h"
#include "../Win32App/Win32App.h"
#include "../Input/KeyboardInput.h"

using namespace DirectX;

Camera::Camera() {
}

void Camera::Initialize() {

	aspect_ratio_ = static_cast<float>(Win32App::GetWindowWidth()) / Win32App::GetWindowHeight();

	// カメラ距離の設定
	distance_ = 20.0f;

	// それぞれ初期化
	eye_ = { 0, 0, -distance_ };
	target_ = { 0, 0, 0 };
	up_vec_ = { 0, 1, 0 };

	// ビュー行列の更新
	UpdateViewMatrix();

	// 透視投影
	UpdateViewProjection();
}

void Camera::Update() {

	UpdateViewMatrix();		// ダーティーフラグを使いたい
	//UpdateViewProjection();	// やらなくてもいい
}

void Camera::BasicCameraMoveTrack(float speed) {

	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F)) {

		if (KeyboardInput::PushKey(DIK_W)) { MoveCameraTrack({ 0.0f, +speed, 0.0f }); } else if (KeyboardInput::PushKey(DIK_S)) { MoveCameraTrack({ 0.0f, -speed, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_D)) { MoveCameraTrack({ +speed, 0.0f, 0.0f }); } else if (KeyboardInput::PushKey(DIK_A)) { MoveCameraTrack({ -speed, 0.0f, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_R)) { MoveCameraTrack({ 0.0f, 0.0f, +speed }); } else if (KeyboardInput::PushKey(DIK_F)) { MoveCameraTrack({ 0.0f, 0.0f, -speed }); }
	}

	if (KeyboardInput::PushKey(DIK_UP)) { MoveEye({ 0.0f, -speed, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_DOWN)) { MoveEye({ 0.0f, +speed, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_LEFT)) { MoveEye({ +speed, 0.0f, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_RIGHT)) { MoveEye({ -speed, 0.0f, 0.0f }); }

	Update();
}

void Camera::BasicCameraMove(float speed) {

	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F)) {

		if (KeyboardInput::PushKey(DIK_W)) { MoveCameraTrack({ 0.0f, +speed, 0.0f }); } else if (KeyboardInput::PushKey(DIK_S)) { MoveCameraTrack({ 0.0f, -speed, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_D)) { MoveCameraTrack({ +speed, 0.0f, 0.0f }); } else if (KeyboardInput::PushKey(DIK_A)) { MoveCameraTrack({ -speed, 0.0f, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_R)) { MoveCameraTrack({ 0.0f, 0.0f, +speed }); } else if (KeyboardInput::PushKey(DIK_F)) { MoveCameraTrack({ 0.0f, 0.0f, -speed }); }
	}

	Update();
}

void Camera::ResetCamera() {

	SetEye({ 0.0f, 0.0f, -GetDistance() });
	SetTarget({ 0.0f, 0.0f, 0.0f });
}

void Camera::UpdateViewMatrix() {

	mat_view_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_vec_));
}

void Camera::UpdateViewProjection() {

	mat_projection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspect_ratio_,
		0.1f, 1000.0f
	);
}

void Camera::MoveCameraTrack(const XMFLOAT3 &move) {

	eye_.x += move.x;
	eye_.y += move.y;
	eye_.z += move.z;

	target_.x += move.x;
	target_.y += move.y;
	target_.z += move.z;
}

void Camera::MoveEye(const XMFLOAT3 &move) {

	eye_.x += move.x;
	eye_.y += move.y;
	eye_.z += move.z;
}
