#pragma once
#include <DirectXMath.h>

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	// 中心座標
	DirectX::XMVECTOR center = { 0, 0, 0, 1 };

	// 半径
	float radius = 1.0f;
};

/// <summary>
/// 平面
/// </summary>
struct Plane
{
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0, 1, 0, 0 };

	// 原点(0, 0, 0)からの距離
	float distance = 0.0f;
};

/// <summary>
/// 三角形
/// </summary>
class Triangle
{
public:

	// 頂点座標3つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;

	// 法線ベクトル
	DirectX::XMVECTOR normal;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Ray
{
	// 始点座標
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };

	// 方向
	DirectX::XMVECTOR dir = { 1, 0 ,0 ,0 };
};

/// <summary>
/// 線分
/// </summary>
struct LineSegment
{
	// 始点座標
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };

	// 終点座標
	DirectX::XMVECTOR end = { 0, 0, 0, 1 };
};

/// <summary>
/// 軸並行バウンディングボックス
/// </summary>
struct AABB
{
	DirectX::XMFLOAT3 near_left_upper;
	DirectX::XMFLOAT3 near_right_downer;
	DirectX::XMFLOAT3 far_left_upper;
	DirectX::XMFLOAT3 far_right_downer;

	DirectX::XMFLOAT3 center = { 0, 0, 0 };

	float width = 1.0f;
	float height = 1.0f;

	float depth = 1.0f;
};