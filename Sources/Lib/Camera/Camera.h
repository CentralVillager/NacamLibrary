#pragma once
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include "../../Lib/Math/Easing/NcmEasing.h"

class Player;

class Camera
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

	struct MatrixConstBufferData
	{
		XMMATRIX mat;
	};

private:

	static constexpr float NORMAL_DIFFER_ = 22.0f;
	static constexpr float NORMAL_DIFFER_Y_ = 12.0f;
	static constexpr float NORMAL_OFFSET_Y = 3.0f;

	static constexpr float CIRCUSE_DIFFER_ = 50.0f;
	static constexpr float CIRCUSE_DIFFER_Y_ = 90.0f;
	static constexpr float CIRCUSE_OFFSET_Y = -10.0f;

private:

	static ComPtr<ID3D12Device> device_;

private:

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> matrix_const_buffer_;
	// ���[���h�s��
	XMMATRIX mat_world_;
	// ��]�s��
	XMMATRIX mat_rot_;
	// �r���[�s��
	XMMATRIX mat_view_;
	// �ˉe�s��
	XMMATRIX mat_projection_;
	// �r���{�[�h�s��
	XMMATRIX mat_billborad_;
	// Y������r���{�[�h�s��
	XMMATRIX mat_billborad_y_;
	// ���_���W
	XMFLOAT3 eye_;
	// �����_���W
	XMFLOAT3 target_;
	// ������x�N�g��
	XMFLOAT3 up_vec_;
	// �J������]�p�x
	XMFLOAT2 angle_;
	// �J��������
	float distance_;
	// �A�X�y�N�g��
	float aspect_ratio_;
	// �e�s��̃|�C���^
	XMMATRIX *parent_mat_;
	// ����p
	float fov_;

	// �~�T�C���p���_�ɂ��邩
	bool is_missile_camera_;

	float differ_;
	float differ_y_;
	float offset_y_;

	ncm_ehandle ease_differ_to_circuse_;
	ncm_ehandle ease_differ_y_to_circuse_;
	ncm_ehandle ease_offset_y_to_circuse_;

	ncm_ehandle ease_differ_to_normal_;
	ncm_ehandle ease_differ_y_to_normal_;
	ncm_ehandle ease_offset_y_to_normal_;

public:

	Camera();

	// �A�N�Z�b�T
	const XMMATRIX &GetMatView() { return mat_view_; }
	const XMMATRIX &GetMatProjection() { return mat_projection_; }
	const XMMATRIX &GetMatBillboard() { return mat_billborad_; }
	const XMMATRIX &GetMatBillboardY() { return mat_billborad_y_; }
	const XMFLOAT3 &GetEye() { return eye_; }
	const XMFLOAT3 &GetTarget() { return target_; }
	const float &GetDistance() { return distance_; }
	const float &GetFOV() { return fov_; }
	const bool GetIsMissileCamera() { return is_missile_camera_; }
	void SetMatView(const XMMATRIX &mat_view) { mat_view_ = mat_view; }
	void SetMatProjection(const XMMATRIX &mat_projection) { mat_projection_ = mat_projection; }
	void SetEye(const XMFLOAT3 &eye) { eye_ = eye; }
	void SetTarget(const XMFLOAT3 &target) { target_ = target; }
	void SetUp(const XMFLOAT3 &up) { up_vec_ = up; }
	void SetDistance(const float &distence) { distance_ = distence; }
	void SetFOV(float fov) { fov_ = fov; }
	void SetDiffer(float differ) { differ_ = differ; }
	void SetDifferY(float differ) { differ_y_ = differ; }
	void SetOffsetY(float offset) { offset_y_ = offset; }
	void SetIsMissileCamera(bool is_missile_cam) { is_missile_camera_ = is_missile_cam; }

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ��{�J�����ړ��@�\
	/// </summary>
	void BasicCameraMoveTrack(float speed);

	/// <summary>
	/// ��{�J�����ړ��@�\
	/// </summary>
	/// <param name="speed"></param>
	void BasicCameraMove(float speed);

	/// <summary>
	/// ���S�Ǐ]�J����
	/// </summary>
	void FollowCameraMove(float speed, Player &player);

	/// <summary>
	/// z���̂ݒǏ]�J����
	/// </summary>
	/// <param name="player"></param>
	void FollowZCameraMove(Player &player);

	/// <summary>
	/// �J�����ʒu�̃��Z�b�g
	/// </summary>
	void ResetCamera();

	void DebugDraw();

public:

	/// <summary>
	/// �r���[�s��̍X�V
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// �ˉe�s��̍X�V
	/// </summary>
	void UpdateViewProjection();

	/// <summary>
	/// �J�����̕��s�ړ�
	/// </summary>
	/// <param name="move"></param>
	void MoveCameraTrack(const XMFLOAT3 &move);

	/// <summary>
	/// ���_�̈ړ�
	/// </summary>
	void MoveEye(const XMFLOAT3 &move);

	/// <summary>
	/// ���_�̕ύX
	/// </summary>
	/// <param name="is_normal">�ʏ펋�_��</param>
	void ChangeView(bool is_normal);

	/// <summary>
	/// �J�����ʒu�̑J��
	/// </summary>
	void TransitionCamera();

private:

	void CreateConstBuffer();
};
