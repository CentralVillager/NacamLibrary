#include "Camera.h"
#include "Win32App.h"

using namespace DirectX;

Camera::Camera() {
}

void Camera::Initialize() {

	aspect_ratio_ = static_cast<float>(Win32App::GetWindowWidth()) / Win32App::GetWindowHeight();

	// �J���������̐ݒ�
	distance_ = 20.0f;

	// ���ꂼ�ꏉ����
	eye_ = { 0, 0, -distance_ };
	target_ = { 0, 0, 0 };
	up_vec_ = { 0, 1, 0 };

	// �r���[�s��̍X�V
	UpdateViewMatrix();

	// �������e
	UpdateViewProjection();
}

void Camera::Update() {

	UpdateViewMatrix();		// �_�[�e�B�[�t���O���g������
	//UpdateViewProjection();	// ���Ȃ��Ă�����
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
