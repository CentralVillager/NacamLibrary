#pragma once
#include <DirectXMath.h>

class Camera {
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	// ビュー行列
	XMMATRIX mat_view_;

	// 射影行列
	XMMATRIX mat_projection_;

	// 視点座標
	XMFLOAT3 eye_;

	// 注視点座標
	XMFLOAT3 target_;

	// 上方向ベクトル
	XMFLOAT3 up_vec_;

	// カメラ角度
	float angle_;

	// カメラ距離
	float distance_;

	// アスペクト比
	float aspect_ratio_;

public:

	Camera();

	// アクセッサ
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
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

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
};
