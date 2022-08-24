//#pragma once
//#include <DirectXMath.h>
//
//struct Quaternion {
//	float x;
//	float y;
//	float z;
//	float w;
//};
//
///// <summary>
///// 成分を指定してクォータニオンを作成
///// </summary>
///// <param name="x"></param>
///// <param name="y"></param>
///// <param name="z"></param>
///// <param name="w"></param>
///// <returns></returns>
//Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
//
///// <summary>
///// 任意軸まわりの回転の指定からクォータニオンを作成
///// </summary>
///// <param name="v"></param>
///// <param name="angle"></param>
///// <returns></returns>
//Quaternion quaternion(const DirectX::XMFLOAT3 &v, float angle);
//
///// <summary>
///// ベクトル v を 姿勢を表すクォータニオン q で回転する
///// </summary>
///// <param name="v"></param>
///// <param name="q"></param>
///// <returns></returns>
//Quaternion quaternion(const DirectX::XMFLOAT3 &v, const Quaternion &q);
//
///// <summary>
///// 内積を求める
///// </summary>
///// <param name="q1">値１</param>
///// <param name="q2">値２</param>
///// <returns>１と２の内積</returns>
//float Dot(const Quaternion &q1, const Quaternion &q2);
//
///// <summary>
///// ノルムを求める
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//float Length(const Quaternion &q);
//
///// <summary>
///// 正規化する
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//Quaternion Normalize(const Quaternion &q);
//
///// <summary>
///// 共役四元数を求める
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//Quaternion Conjugate(const Quaternion &q);
//
///// <summary>
///// クォータニオンを球面線形補間する
///// </summary>
///// <param name="q1">補間開始地点</param>
///// <param name="p2">補間終了地点</param>
///// <param name="t">時間</param>
///// <returns>補間結果</returns>
//Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, float t);
//
///// <summary>
///// クォータニオンを線形補間する
///// </summary>
///// <param name="q1">補間開始地点</param>
///// <param name="q2">補間終了地点</param>
///// <param name="t">時間</param>
///// <returns><補間結果/returns>
//Quaternion Lerp(const Quaternion &q1, const Quaternion &q2, float t);
//
///// <summary>
///// クォータニオンから回転行列に変換する
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//DirectX::XMMATRIX Rotate(const Quaternion &q);
//
///// <summary>
///// 回転軸からクォータニオンを求める
///// </summary>
///// <param name="m"></param>
///// <returns></returns>
//Quaternion quaternion(const DirectX::XMMATRIX &m);
//
///// <summary>
///// 回転軸の算出
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//DirectX::XMFLOAT3 GetAxis(const Quaternion &q);
//
//// 単項演算子オーバーロード
//Quaternion operator + (const Quaternion &q);
//Quaternion operator - (const Quaternion &q);
//
//// 代入演算子オーバーロード
//Quaternion &operator += (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator -= (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator *= (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator *= (Quaternion &q, float s);
//Quaternion &operator /= (Quaternion &q, float s);
//
//// 2項演算子オーバーロード
//Quaternion operator + (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator - (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator * (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator * (const Quaternion &q, float s);
//Quaternion operator * (float s, const Quaternion &q);
//Quaternion operator / (const Quaternion &q, float s);
