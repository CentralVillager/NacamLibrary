#pragma once
#include "../Collision/CollisionPrimitive.h"

class Collision {
	using XMVECTOR = DirectX::XMVECTOR;

public:

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere1">球1</param>
	/// <param name="sphere2">球2</param>
	/// <returns>当たっているか否か</returns>
	static bool CheckSphere2Sphere(const Sphere &sphere1, const Sphere &sphere2);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere &sphere, const Plane &plane, XMVECTOR *inter = nullptr);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点</param>
	/// <returns></returns>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest);

	/// <summary>
	/// 球と法線付き三角形の当たりチェック
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="triangle"></param>
	/// <param name="inter"></param>
	/// <returns></returns>
	static bool CheckSphere2Triangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray &ray, const Plane &plane, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray &ray, const Triangle &triangle, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	///	レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離（出力）</param>
	/// <param name="inter">交点（出力）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray &ray, const Sphere &sphere, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// 線分と円の当たり判定
	/// </summary>
	/// <param name="line"></param>
	/// <param name="sphere"></param>
	/// <returns></returns>
	static bool CheckLine2Circle(const LineSegment &line, const Sphere &sphere);

	/// <summary>
	/// 軸並行バウンディングボックスでの当たり判定
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <returns></returns>
	static bool CheckAABB2AABB(const AABB &pos1, const AABB &pos2);

	static inline bool CheckMinus(const float &value) {

		if (value < 0) { return true; }

		return false;
	}
};
