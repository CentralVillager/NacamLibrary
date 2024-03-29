#include "Camera.h"
#include <cmath>
#include "../Win32App/Win32App.h"
#include "../Input/KeyboardInput.h"
#include "../DirectX/DirectXBase.h"
#include "../../Lib/DebugUtill/NcmImGui.h"
#include "../../App/Player/Player.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> Camera::device_ = nullptr;

Camera::Camera() :
	matrix_const_buffer_(),
	mat_world_(),
	mat_rot_(),
	mat_view_(),
	mat_projection_(),
	mat_billborad_(),
	mat_billborad_y_(),
	eye_(),
	target_(),
	up_vec_(),
	angle_(),
	distance_(),
	aspect_ratio_(),
	parent_mat_(),
	fov_(),
	is_missile_camera_(),
	differ_(),
	differ_y_(),
	offset_y_(),
	ease_differ_to_circuse_(),
	ease_differ_y_to_circuse_(),
	ease_offset_y_to_circuse_(),
	ease_differ_to_normal_(),
	ease_differ_y_to_normal_(),
	ease_offset_y_to_normal_()
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

	using enum NcmEaseType;
	NcmEaseType type = OutCubic;
	float rate = 0.01f;

	NcmEaseDesc desc{};
	desc.init_value = NORMAL_DIFFER_;
	desc.total_move = CIRCUSE_DIFFER_ - NORMAL_DIFFER_;
	desc.ease_type = type;
	desc.t_rate = rate;
	ease_differ_to_circuse_ = NcmEasing::RegisterEaseData(desc);

	desc.init_value = NORMAL_DIFFER_Y_;
	desc.total_move = CIRCUSE_DIFFER_Y_ - NORMAL_DIFFER_Y_;
	ease_differ_y_to_circuse_ = NcmEasing::RegisterEaseData(desc);

	desc.init_value = NORMAL_OFFSET_Y;
	desc.total_move = CIRCUSE_OFFSET_Y - NORMAL_OFFSET_Y;
	ease_offset_y_to_circuse_ = NcmEasing::RegisterEaseData(desc);

	desc.init_value = CIRCUSE_DIFFER_;
	desc.total_move = NORMAL_DIFFER_ - CIRCUSE_DIFFER_;
	ease_differ_to_normal_ = NcmEasing::RegisterEaseData(desc);

	desc.init_value = CIRCUSE_DIFFER_Y_;
	desc.total_move = NORMAL_DIFFER_Y_ - CIRCUSE_DIFFER_Y_;
	ease_differ_y_to_normal_ = NcmEasing::RegisterEaseData(desc);

	desc.init_value = CIRCUSE_OFFSET_Y;
	desc.total_move = NORMAL_OFFSET_Y - CIRCUSE_OFFSET_Y;
	ease_offset_y_to_normal_ = NcmEasing::RegisterEaseData(desc);
}

void Camera::Update()
{
	UpdateViewMatrix();		// ダーティーフラグを使いたい
	UpdateViewProjection();

	TransitionCamera();

	HRESULT result = S_FALSE;

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
}

void Camera::FollowCameraMove(float speed, Player &player)
{
	// 上ベクトルを計算
	XMFLOAT3 up_vec = { 0, 1, 0 };
	XMVECTOR up_temp = XMVector3Transform(XMLoadFloat3(&up_vec), player.GetRotMat());
	XMStoreFloat3(&up_vec, up_temp);

	XMFLOAT3 eye_result{};
	eye_result.x = player.GetPos().x - player.GetFwdVec().x * differ_ + up_vec.x * differ_y_;
	eye_result.y = player.GetPos().y - player.GetFwdVec().y * differ_ + up_vec.y * differ_y_;
	eye_result.z = player.GetPos().z - player.GetFwdVec().z * differ_ + up_vec.z * differ_y_;

	eye_ = eye_result;

	XMFLOAT3 tgt_result{};
	tgt_result.x = player.GetPos().x + player.GetFwdVec().x * differ_;
	tgt_result.y = player.GetPos().y + player.GetFwdVec().y * differ_;
	tgt_result.z = player.GetPos().z + player.GetFwdVec().z * differ_;

	tgt_result.y -= offset_y_;

	// 注視点を更新
	target_ = tgt_result;

	// 上ベクトルを更新
	up_vec_ = up_vec;
}

void Camera::FollowZCameraMove(Player &player)
{
	// 上ベクトルを計算
	XMFLOAT3 up_vec = { 0, 1, 0 };
	XMVECTOR up_temp = XMVector3Transform(XMLoadFloat3(&up_vec), player.GetRotMat());
	XMStoreFloat3(&up_vec, up_temp);

	XMFLOAT3 eye_result{};
	eye_result.x = player.GetFwdVec().x * differ_ + up_vec.x * differ_y_;
	eye_result.y = player.GetFwdVec().y * differ_ + up_vec.y * differ_y_;
	eye_result.z = player.GetPos().z - player.GetFwdVec().z * differ_ + up_vec.z * differ_y_;

	eye_ = eye_result;

	XMFLOAT3 tgt_result{};
	tgt_result.x = player.GetFwdVec().x * differ_;
	tgt_result.y = player.GetFwdVec().y * differ_;
	tgt_result.z = player.GetPos().z + player.GetFwdVec().z * differ_;

	tgt_result.y -= offset_y_;

	// 注視点を更新
	target_ = tgt_result;

	// 上ベクトルを更新
	up_vec_ = up_vec;
}

void Camera::ResetCamera()
{
	SetEye({ 0.0f, 0.0f, -distance_ });
	SetTarget({ 0.0f, 0.0f, 0.0f });
}

void Camera::DebugDraw()
{
	ImGui::Checkbox("MissileCamera", &is_missile_camera_);

	ImGui::DragFloat("FOV", &fov_, 0.1f, 0.1f, 179.99f);

	ImGui::DragFloat("Differ", &differ_, 0.1f, 0, 200.0f);
	ImGui::DragFloat("DifferY", &differ_y_, 0.1f, 0, 200.0f);
	ImGui::DragFloat("OffsetY", &offset_y_, 0.1f, -200.0f, 200.0f);
	ImGui::Text("target : (%f, %f, %f)", target_.x, target_.y, target_.z);
}

void Camera::UpdateViewMatrix()
{
	// 視点座標
	XMVECTOR eye_pos = XMLoadFloat3(&eye_);
	// 注視点座標
	XMVECTOR target_pos = XMLoadFloat3(&target_);
	// （仮の）上方向
	XMVECTOR up_vec = XMLoadFloat3(&up_vec_);

	// カメラZ軸（視線方向）
	XMVECTOR camera_axis_z = XMVectorSubtract(target_pos, eye_pos);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(camera_axis_z, XMVectorZero()));
	assert(!XMVector3IsInfinite(camera_axis_z));
	assert(!XMVector3Equal(up_vec, XMVectorZero()));
	assert(!XMVector3IsInfinite(up_vec));
	// ベクトルを正規化
	camera_axis_z = XMVector3Normalize(camera_axis_z);

	// カメラのX軸（右方向）
	XMVECTOR camera_axis_x;
	// X軸は上方向→Z軸の外積で求まる
	camera_axis_x = XMVector3Cross(up_vec, camera_axis_z);
	// ベクトルを正規化
	camera_axis_x = XMVector3Normalize(camera_axis_x);

	// カメラのY軸（上方向）
	XMVECTOR camera_axis_y;
	// Y軸はZ軸→X軸の外積で求まる
	camera_axis_y = XMVector3Cross(camera_axis_z, camera_axis_x);

	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのカメラの右方向、上方向、前方向）
	// カメラ座標系→ワールド座標系の変換行列
	mat_rot_.r[0] = camera_axis_x;
	mat_rot_.r[1] = camera_axis_y;
	mat_rot_.r[2] = camera_axis_z;
	mat_rot_.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列（逆回転）を計算
	mat_view_ = XMMatrixTranspose(mat_rot_);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverse_eye_pos = XMVectorNegate(eye_pos);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR t_x = XMVector3Dot(camera_axis_x, reverse_eye_pos);	// X成分
	XMVECTOR t_y = XMVector3Dot(camera_axis_y, reverse_eye_pos);	// Y成分
	XMVECTOR t_z = XMVector3Dot(camera_axis_z, reverse_eye_pos);	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(t_x.m128_f32[0], t_y.m128_f32[1], t_z.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	mat_view_.r[3] = translation;

	/* --ビルボード -- */

	// ビルボード行列
	mat_billborad_.r[0] = camera_axis_x;
	mat_billborad_.r[1] = camera_axis_y;
	mat_billborad_.r[2] = camera_axis_z;
	mat_billborad_.r[3] = XMVectorSet(0, 0, 0, 1);

	/* -- Y軸周りビルボード -- */

	// カメラX軸、Y軸、Z軸
	XMVECTOR y_bill_cam_axis_x, y_bill_cam_axis_y, y_bill_cam_axis_z;

	// X軸は共通
	y_bill_cam_axis_x = camera_axis_x;
	// Y軸はワールド座標系のY軸
	y_bill_cam_axis_y = XMVector3Normalize(up_vec);
	// Z軸はX軸→Y軸の外積で求まる
	y_bill_cam_axis_z = XMVector3Cross(y_bill_cam_axis_x, y_bill_cam_axis_y);

	// Y軸回りビルボード行列
	mat_billborad_y_.r[0] = y_bill_cam_axis_x;
	mat_billborad_y_.r[1] = y_bill_cam_axis_y;
	mat_billborad_y_.r[2] = y_bill_cam_axis_z;
	mat_billborad_y_.r[3] = XMVectorSet(0, 0, 0, 1);
}

void Camera::UpdateViewProjection()
{
	mat_projection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(fov_),
		aspect_ratio_,
		0.1f, 6000.0f
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

void Camera::ChangeView(bool is_normal)
{
	if (is_normal)
	{
		differ_ = NORMAL_DIFFER_;
		differ_y_ = NORMAL_DIFFER_Y_;
		offset_y_ = NORMAL_OFFSET_Y;
	}
	else
	{
		differ_ = CIRCUSE_DIFFER_;
		differ_y_ = CIRCUSE_DIFFER_Y_;
		offset_y_ = CIRCUSE_OFFSET_Y;
	}
}

void Camera::TransitionCamera()
{
	if (is_missile_camera_)
	{
		/*NcmEasing::UpdateValue(ease_differ_to_circuse_);
		NcmEasing::UpdateValue(ease_differ_y_to_circuse_);
		NcmEasing::UpdateValue(ease_offset_y_to_circuse_);

		differ_ = NcmEasing::GetValue(ease_differ_to_circuse_);
		differ_y_ = NcmEasing::GetValue(ease_differ_y_to_circuse_);
		offset_y_ = NcmEasing::GetValue(ease_offset_y_to_circuse_);

		NcmEasing::ResetTime(ease_differ_to_normal_);
		NcmEasing::ResetTime(ease_differ_y_to_normal_);
		NcmEasing::ResetTime(ease_offset_y_to_normal_);*/
	}
	else
	{
		NcmEasing::UpdateValue(ease_differ_to_normal_);
		NcmEasing::UpdateValue(ease_differ_y_to_normal_);
		NcmEasing::UpdateValue(ease_offset_y_to_normal_);

		differ_ = NcmEasing::GetValue(ease_differ_to_normal_);
		differ_y_ = NcmEasing::GetValue(ease_differ_y_to_normal_);
		offset_y_ = NcmEasing::GetValue(ease_offset_y_to_normal_);

		NcmEasing::ResetTime(ease_differ_to_circuse_);
		NcmEasing::ResetTime(ease_differ_y_to_circuse_);
		NcmEasing::ResetTime(ease_offset_y_to_circuse_);
	}
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
