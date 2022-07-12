#include "Camera.h"
#include "Win32App.h"

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
