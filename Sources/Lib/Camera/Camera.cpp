#include "Camera.h"
#include "../Win32App/Win32App.h"
#include "../Input/KeyboardInput.h"
#include "../DirectXBase/DirectXBase.h"
#include "../../App/Debug/NcmImGui.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> Camera::device_ = nullptr;

Camera::Camera()
{
	device_ = DirectXBase::GetInstance()->GetDevice().Get();
}

void Camera::Initialize()
{
	CreateConstBuffer();

	aspect_ratio_ = static_cast<float>(Win32App::SIZE_.x) / Win32App::SIZE_.y;

	// カメラ距離の設定
	distance_ = 20.0f;

	// それぞれ初期化
	eye_ = { 0, 0, -distance_ };
	target_ = { 0, 0, 0 };
	up_vec_ = { 0, 1, 0 };

	fov_ = 60.0f;

	// ビュー行列の更新
	UpdateViewMatrix();

	// 透視投影
	UpdateViewProjection();
}

void Camera::Update()
{
	UpdateViewMatrix();		// ダーティーフラグを使いたい
	UpdateViewProjection();	// やらなくてもいい

	HRESULT result = S_FALSE;

	mat_world_ = XMMatrixIdentity();
	MatrixConstBufferData *matrix_const_map = nullptr;
	result = matrix_const_buffer_->Map(0, nullptr, reinterpret_cast<void **>(&matrix_const_map));
	matrix_const_map->mat = mat_world_ * mat_view_ * mat_projection_;
	matrix_const_buffer_->Unmap(0, nullptr);
}

void Camera::BasicCameraMoveTrack(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{
		if (KeyboardInput::PushKey(DIK_W)) { MoveCameraTrack({ 0.0f, +speed, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_S)) { MoveCameraTrack({ 0.0f, -speed, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_D)) { MoveCameraTrack({ +speed, 0.0f, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_A)) { MoveCameraTrack({ -speed, 0.0f, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_R)) { MoveCameraTrack({ 0.0f, 0.0f, +speed }); }
		else if (KeyboardInput::PushKey(DIK_F)) { MoveCameraTrack({ 0.0f, 0.0f, -speed }); }
	}

	if (KeyboardInput::PushKey(DIK_UP)) { MoveEye({ 0.0f, -speed, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_DOWN)) { MoveEye({ 0.0f, +speed, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_LEFT)) { MoveEye({ +speed, 0.0f, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_RIGHT)) { MoveEye({ -speed, 0.0f, 0.0f }); }

	Update();
}

void Camera::BasicCameraMove(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{
		if (KeyboardInput::PushKey(DIK_W)) { MoveCameraTrack({ 0.0f, +speed, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_S)) { MoveCameraTrack({ 0.0f, -speed, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_D)) { MoveCameraTrack({ +speed, 0.0f, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_A)) { MoveCameraTrack({ -speed, 0.0f, 0.0f }); }

		if (KeyboardInput::PushKey(DIK_R)) { MoveCameraTrack({ 0.0f, 0.0f, +speed }); }
		else if (KeyboardInput::PushKey(DIK_F)) { MoveCameraTrack({ 0.0f, 0.0f, -speed }); }
	}

	Update();
}

void Camera::MoveXY(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A))
	{
		if (KeyboardInput::PushKey(DIK_W)) { MoveCameraTrack({ 0.0f, 0.0f, +speed }); }
		else if (KeyboardInput::PushKey(DIK_S)) { MoveCameraTrack({ 0.0f, 0.0f, -speed }); }

		if (KeyboardInput::PushKey(DIK_D)) { MoveCameraTrack({ +speed, 0.0f, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_A)) { MoveCameraTrack({ -speed, 0.0f, 0.0f }); }
	}

	Update();
}

void Camera::ResetCamera()
{
	SetEye({ 0.0f, 0.0f, -distance_ });
	SetTarget({ 0.0f, 0.0f, 0.0f });
}

void Camera::DebugDraw()
{
	ImGui::DragFloat("FOV", &fov_, 0.1f, 0.1f, 179.9f);
}

void Camera::UpdateViewMatrix()
{
	mat_view_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_vec_));

	if (parent_mat_ != nullptr)
	{
		//mat_view_ *= parent_mat_;
	}
}

void Camera::UpdateViewProjection()
{
	mat_projection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(fov_),
		aspect_ratio_,
		0.1f, 10000.0f
	);
}

void Camera::MoveCameraTrack(const XMFLOAT3 &move)
{
	eye_.x += move.x;
	eye_.y += move.y;
	eye_.z += move.z;

	target_.x += move.x;
	target_.y += move.y;
	target_.z += move.z;
}

void Camera::MoveEye(const XMFLOAT3 &move)
{
	eye_.x += move.x;
	eye_.y += move.y;
	eye_.z += move.z;
}

void Camera::CreateConstBuffer()
{
	HRESULT result;

	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(MatrixConstBufferData)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&matrix_const_buffer_));
	matrix_const_buffer_->SetName(L"CameraMatrixConstBuffer");
}
