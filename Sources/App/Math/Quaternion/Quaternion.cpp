//#include "Quaternion.h"
//#include <cmath>
//
//using namespace DirectX;
//
//Quaternion quaternion(float x, float y, float z, float w) {
//
//	Quaternion result{ x, y, z, w };
//	return result;
//}
//
//Quaternion quaternion(const DirectX::XMFLOAT3 &v, float angle) {
//
//	float _sin = sin(angle / 2.0f);
//	return quaternion(_sin * v.x, _sin * v.y, _sin * v.z, cos(angle / 2.0f));
//}
//
//Quaternion quaternion(const XMFLOAT3 &v, const Quaternion &q) {
//
//	Quaternion qq = Conjugate(q);					// qの共役四元数を作成する
//	Quaternion qv = quaternion(v.x, v.y, v.z, 0);	// ベクトルvをクォータニオン化する
//
//	return q * qv * qq;		// ベクトルを回転したクォータニオンを返す
//}
//
//float Dot(const Quaternion &q1, const Quaternion &q2) {
//
//	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
//}
//
//float Length(const Quaternion &q) {
//
//	return static_cast<float>(sqrt(Dot(q, q)));
//}
//
//Quaternion Normalize(const Quaternion &q) {
//
//	Quaternion result = q;
//	float len = Length(result);
//	if (len != 0) { result /= len; }
//	return result;
//}
//
//Quaternion Conjugate(const Quaternion &q) {
//
//	Quaternion result = { -q.x, -q.y, -q.z, q.w };
//	return result;
//}
//
//Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, float t) {
//
//	float cos = Dot(q1, q2);
//	Quaternion t2 = q2;
//
//	if (cos < 0.0f) {
//
//		cos = -cos;
//		t2 = -q2;
//	}
//
//	float k0 = 1.0f - t;
//	float k1 = t;
//
//	if ((1.0f - cos) > 0.001f) {
//
//		float theta = static_cast<float>(acos(cos));
//		k0 = static_cast<float>((sin(theta * k0) / sin(theta)));
//		k1 = static_cast<float>((sin(theta * k1) / sin(theta)));
//	}
//
//	return q1 * k0 + t2 * k1;
//}
//
//Quaternion Lerp(const Quaternion &q1, const Quaternion &q2, float t) {
//
//	float cos = Dot(q1, q2);
//	Quaternion t2 = q2;
//
//	if (cos < 0.0f) {
//
//		cos = -cos;
//		t2 = -q2;
//	}
//
//	float k0 = 1.0f - t;
//	float k1 = t;
//
//	return q1 * k0 + t2 * k1;
//}
//
//XMMATRIX Rotate(const Quaternion &q) {
//
//	float xx = q.x * q.x * 2.0f;
//	float yy = q.y * q.y * 2.0f;
//	float zz = q.z * q.z * 2.0f;
//	float xy = q.x * q.y * 2.0f;
//	float xz = q.x * q.z * 2.0f;
//	float yz = q.y * q.z * 2.0f;
//	float wx = q.w * q.x * 2.0f;
//	float wy = q.w * q.y * 2.0f;
//	float wz = q.w * q.z * 2.0f;
//
//	XMMATRIX result = {
//
//		1.0f - yy - zz,		xy + wz,		xz - wy,		0.0f,
//		xy - wz,			1.0f - xx - zz,	yz + wx,		0.0f,
//		xz + wy,			yz - wx,		1.0f - xx - yy,	0.0f,
//		0.0f,				0.0f,			0.0f,			1.0f
//	};
//
//	return result;
//}
//
//Quaternion quaternion(const XMMATRIX &m) {
//
//	Quaternion result;
//	float tr = m.r[0][0] + m.m[1][1] + m.m[2][2] + m.m[3][3];
//
//	if (tr >= 1.0f) {
//
//		float four_d = 2.0f * sqrt(tr);
//		result.x = (m.m[1][2] - m.m[2][1]) / four_d;
//		result.y = (m.m[2][0] - m.m[0][2]) / four_d;
//		result.z = (m.m[0][1] - m.m[1][0]) / four_d;
//		result.w = four_d / 4.0f;
//		return result;
//	}
//
//	int i = 0;
//	if (m.m[0][0] <= m.m[1][1]) { i = 1; }
//	if (m.m[2][2] > m.m[i][i]) { i = 2; }
//	int j = (i + 1) % 3;
//	int k = (j + 1) % 3;
//	tr = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;
//	float four_d = 2.0f * sqrt(tr);
//	float qa[4];
//	qa[i] = four_d / 4.0f;
//	qa[j] = (m.m[j][i] + m.m[i][j]) / four_d;
//	qa[k] = (m.m[k][i] + m.m[i][k]) / four_d;
//	qa[3] = (m.m[j][k] - m.m[k][j]) / four_d;
//	result.x = qa[0];
//	result.y = qa[1];
//	result.z = qa[2];
//	result.w = qa[3];
//	return result;
//}
//
//XMFLOAT3 GetAxis(const Quaternion &q) {
//
//	XMFLOAT3 result;
//
//	float x = q.x;
//	float y = q.y;
//	float z = q.z;
//	float len = Length(q);
//
//	result.x = x / len;
//	result.y = y / len;
//	result.z = z / len;
//
//	return result;
//}
//
//Quaternion operator+(const Quaternion &q) {
//	return q;
//}
//
//Quaternion operator-(const Quaternion &q) {
//	Quaternion result = { -q.x, -q.y, -q.z, -q.w };
//	return result;
//}
//
//Quaternion &operator+=(Quaternion &q1, const Quaternion &q2) {
//	q1.x += q2.x;
//	q1.y += q2.y;
//	q1.z += q2.z;
//	q1.w += q2.w;
//	return q1;
//}
//
//Quaternion &operator-=(Quaternion &q1, const Quaternion &q2) {
//	q1.x -= q2.x;
//	q1.y -= q2.y;
//	q1.z -= q2.z;
//	q1.w -= q2.w;
//	return q1;
//}
//
//Quaternion &operator*=(Quaternion &q1, const Quaternion &q2) {
//	Quaternion result = {
//		q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
//		-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
//		q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
//		-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w,
//	};
//	q1 = result;
//	return q1;
//}
//
//Quaternion &operator*=(Quaternion &q, float s) {
//	q.x *= s;
//	q.y *= s;
//	q.z *= s;
//	q.w *= s;
//	return q;
//}
//
//Quaternion &operator/=(Quaternion &q, float s) {
//	return q *= 1.0f / s;
//}
//
//Quaternion operator+(const Quaternion &q1, const Quaternion &q2) {
//	Quaternion result = q1;
//	return result += q2;
//}
//
//Quaternion operator-(const Quaternion &q1, const Quaternion &q2) {
//	Quaternion result = q1;
//	return result -= q2;
//}
//
//Quaternion operator*(const Quaternion &q1, const Quaternion &q2) {
//	Quaternion result = q1;
//	return result *= q2;
//}
//
//Quaternion operator*(const Quaternion &q, float s) {
//	Quaternion result = q;
//	return result *= s;
//}
//
//Quaternion operator*(float s, const Quaternion &q) {
//	Quaternion result = q;
//	return result *= s;
//}
//
//Quaternion operator/(const Quaternion &q, float s) {
//	Quaternion result = q;
//	return result /= s;
//}
