#pragma once
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <d3d12.h>

class Camera
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

	struct MatrixConstBufferData
	{
		XMMATRIX mat;
	};

private:

	static ComPtr<ID3D12Device> device_;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> matrix_const_buffer_;

	// ���[���h�s��
	XMMATRIX mat_world_;

	// �r���[�s��
	XMMATRIX mat_view_;

	// �ˉe�s��
	XMMATRIX mat_projection_;

	// ���_���W
	XMFLOAT3 eye_;

	// �����_���W
	XMFLOAT3 target_;

	// ������x�N�g��
	XMFLOAT3 up_vec_;

	// �J�����p�x
	float angle_;

	// �J��������
	float distance_;

	// �A�X�y�N�g��
	float aspect_ratio_;

	// �e�s��̃|�C���^
	XMMATRIX *parent_mat_;

	// ����p
	float fov_;

public:

	Camera();

	// �A�N�Z�b�T
	const XMMATRIX &GetMatView() { return mat_view_; }
	const XMMATRIX &GetMatProjection() { return mat_projection_; }
	const XMFLOAT3 &GetEye() { return eye_; }
	const XMFLOAT3 &GetTarget() { return target_; }
	const float &GetDistance() { return distance_; }
	void SetMatView(const XMMATRIX &mat_view) { mat_view_ = mat_view; }
	void SetMatProjection(const XMMATRIX &mat_projection) { mat_projection_ = mat_projection; }
	void SetEye(const XMFLOAT3 &eye) { eye_ = eye; }
	void SetTarget(const XMFLOAT3 &target) { target_ = target; }
	void SetUp(const XMFLOAT3 &up) { up_vec_ = up; }
	void SetDistance(const float &distence) { distance_ = distence; }

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

	void MoveXY(float speed);

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

private:

	void CreateConstBuffer();
};
