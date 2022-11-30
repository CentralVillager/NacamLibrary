#pragma once
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <d3d12.h>

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

	static ComPtr<ID3D12Device> device_;

	// 定数バッファ
	ComPtr<ID3D12Resource> matrix_const_buffer_;
	// ワールド行列
	XMMATRIX mat_world_;
	// 回転行列
	XMMATRIX mat_rot_;
	// ビュー行列
	XMMATRIX mat_view_;
	// 射影行列
	XMMATRIX mat_projection_;
	// ビルボード行列
	XMMATRIX mat_billborad_;
	// Y軸周りビルボード行列
	XMMATRIX mat_billborad_y_;
	// 視点座標
	XMFLOAT3 eye_;
	// 注視点座標
	XMFLOAT3 target_;
	// 上方向ベクトル
	XMFLOAT3 up_vec_;
	// カメラ回転角度
	XMFLOAT2 angle_;
	// カメラ距離
	float distance_;
	// アスペクト比
	float aspect_ratio_;
	// 親行列のポインタ
	XMMATRIX *parent_mat_;
	// 視野角
	float fov_;

	float differ_ = 22.0f;
	float differ_y_ = 12.0f;
	float offset_y_ = 3.0f;

public:

	Camera();

	// アクセッサ
	const XMMATRIX &GetMatView() { return mat_view_; }
	const XMMATRIX &GetMatProjection() { return mat_projection_; }
	const XMMATRIX &GetMatBillboard() { return mat_billborad_; }
	const XMMATRIX &GetMatBillboardY() { return mat_billborad_y_; }
	const XMFLOAT3 &GetEye() { return eye_; }
	const XMFLOAT3 &GetTarget() { return target_; }
	const float &GetDistance() { return distance_; }
	const float &GetFOV() { return fov_; }
	void SetMatView(const XMMATRIX &mat_view) { mat_view_ = mat_view; }
	void SetMatProjection(const XMMATRIX &mat_projection) { mat_projection_ = mat_projection; }
	void SetEye(const XMFLOAT3 &eye) { eye_ = eye; }
	void SetTarget(const XMFLOAT3 &target) { target_ = target; }
	void SetUp(const XMFLOAT3 &up) { up_vec_ = up; }
	void SetDistance(const float &distence) { distance_ = distence; }
	void SetFOV(float fov) { fov_ = fov; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 基本カメラ移動機能
	/// </summary>
	void BasicCameraMoveTrack(float speed);

	/// <summary>
	/// 基本カメラ移動機能
	/// </summary>
	/// <param name="speed"></param>
	void BasicCameraMove(float speed);

	/// <summary>
	/// 追従カメラ
	/// </summary>
	void FollowCameraMove(float speed, Player &player);

	/// <summary>
	/// カメラ位置のリセット
	/// </summary>
	void ResetCamera();

	void DebugDraw();

public:

	/// <summary>
	/// ビュー行列の更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// 射影行列の更新
	/// </summary>
	void UpdateViewProjection();

	/// <summary>
	/// カメラの平行移動
	/// </summary>
	/// <param name="move"></param>
	void MoveCameraTrack(const XMFLOAT3 &move);

	/// <summary>
	/// 視点の移動
	/// </summary>
	void MoveEye(const XMFLOAT3 &move);

private:

	void CreateConstBuffer();
};
