#pragma once
#include <DirectXMath.h>

using Matrix4 = DirectX::XMMATRIX;

class Vector3;

// 単位行列を求める
Matrix4 Identity();
// 拡大縮小行列の設定
Matrix4 Scale(const Vector3 &s);
// 回転行列の設定
Matrix4 RotationX(float angle);
Matrix4 RotationY(float angle);
Matrix4 RotationZ(float angle);
// 平行移動行列の作成
Matrix4 Translate(const Vector3 &t);
// 座標変換（ベクトルと行列の掛け算をする）
Vector3 Transform(const Vector3 &v, const Matrix4 &m);
// 代入演算子オーバーロード
Matrix4 &operator *= (Matrix4 &m1, const Matrix4 &m2);
// 2項演算子オーバーロード
Matrix4 operator * (const Matrix4 &m1, const Matrix4 &m2);
Vector3 operator * (const Vector3 &v, const Matrix4 &m);
