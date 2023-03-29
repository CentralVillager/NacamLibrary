#include "Collision.h"
#include "../Utility/NcmUtil.h"

using namespace DirectX;
using namespace NcmUtill;

bool Collision::CheckSphere2Sphere(const Sphere &sphere1, const Sphere &sphere2)
{
	// 2�̋��̒��S���W�����ԃx�N�g�������߂�
	XMVECTOR vec =
	{
		(sphere1.center.m128_f32[0] - sphere2.center.m128_f32[0]),
		(sphere1.center.m128_f32[1] - sphere2.center.m128_f32[1]),
		(sphere1.center.m128_f32[2] - sphere2.center.m128_f32[2])
	};

	// �x�N�g���̒�����2��i2�_�Ԃ̋����̓��j�����߂�
	float sq_length = (vec.m128_f32[0] * vec.m128_f32[0]) + (vec.m128_f32[1] * vec.m128_f32[1]) + (vec.m128_f32[2] * vec.m128_f32[2]);

	// 2�̋��̔��a�̍��v�����߂�
	float r = sphere1.radius + sphere2.radius;

	// 2�_�Ԃ̋�����2�� < ���a�̍��v��2��Ȃ�A2�̋��͌������Ă���
	return sq_length < (r *r);
}

bool Collision::CheckSphere2Plane(const Sphere &sphere, const Plane &plane, XMVECTOR *inter)
{
	// ���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR dist_v = XMVector3Dot(sphere.center, plane.normal);

	// ���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = dist_v.m128_f32[0] - plane.distance;

	// �����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabs(dist) > sphere.radius) { return false; }

	// �[����_���v�Z
	if (inter)
	{
		// ���ʏ�̍ŋߐړ_���[����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];

	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);

	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];

	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];

	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter)
{
	XMVECTOR p;

	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	// �_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;

	// �����̓������߂�
	// (�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	v = XMVector3Dot(v, v);

	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > sphere.radius * sphere.radius) { return false; }

	// �[����_���v�Z
	if (inter)
	{
		// �O�p�`��̍ŋߐړ_p���[����_�Ƃ���
		*inter = p;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray &ray, const Plane &plane, float *distance, DirectX::XMVECTOR *inter)
{
	const float epsilon = 1.0e-5f;	// �덷�z���p�̋ɏ��Ȓl

	// �ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];

	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) { return false; }

	// �n�_�ƌ��_�̋����i���ʂ̖@�������j
	// �ʖ@���ƃ��C�̎��_���W�i�ʒu�x�N�g���j�̓���
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];

	// �n�_�ƕ��ʂ̋����i���ʂ̖@�������j
	float dist = d2 - plane.distance;

	// �n�_�ƕ��ʂ̋����i���C�����j
	float t = dist / -d1;

	// ��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0) { *distance = t; }

	// ��_���v�Z
	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

bool Collision::CheckRay2Triangle(const Ray &ray, const Triangle &triangle, float *distance, DirectX::XMVECTOR *inter)
{
	// �O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR inter_plane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];

	// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRay2Plane(ray, plane, distance, &inter_plane)) { return false; }

	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;	// �덷�z���p�̋ɏ��Ȓl
	XMVECTOR m;

	// ��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - inter_plane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);

	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }

	// ��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - inter_plane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);

	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }

	// ��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - inter_plane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);

	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }

	// �����Ȃ̂ŁA�������Ă���
	if (inter)
	{
		*inter = inter_plane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray &ray, const Sphere &sphere, float *distance, DirectX::XMVECTOR *inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;

	// ray�̎n�_��sphere�̊O���ɂ���(c > 0), ray��sphere���痣��Ă����������w���Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;

	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) { return false; }

	// ���C�͋��ƌ������Ă���B
	// ��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);

	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0) { t = 0.0f; }
	if (distance) { *distance = t; }

	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

bool Collision::CheckLine2Circle(const LineSegment &line, const Sphere &sphere)
{
	// �x�N�g�����擾
	XMVECTOR s_c_vec = line.start - sphere.center;
	XMVECTOR e_c_vec = line.end - sphere.center;
	XMVECTOR line_vec = line.start - line.end;

	// �������~�낵�������v�Z
	float dist = s_c_vec.m128_f32[0] * line_vec.m128_f32[1] - s_c_vec.m128_f32[1] * line_vec.m128_f32[0];
	dist /= static_cast<float>(sqrt(line_vec.m128_f32[0] * line_vec.m128_f32[0] + line_vec.m128_f32[1] * line_vec.m128_f32[1] + line_vec.m128_f32[2] * line_vec.m128_f32[2]));

	// ���̔��a�̂ق�����������Γ������Ă��Ȃ�
	if (sphere.radius < fabs(dist)) { return false; }

	// ���ς��v�Z
	float dot1 = line_vec.m128_f32[0] * s_c_vec.m128_f32[0] + line_vec.m128_f32[1] * s_c_vec.m128_f32[1] + line_vec.m128_f32[2] * s_c_vec.m128_f32[2];
	float dot2 = line_vec.m128_f32[0] * e_c_vec.m128_f32[0] + line_vec.m128_f32[1] * e_c_vec.m128_f32[1] + line_vec.m128_f32[2] * e_c_vec.m128_f32[2];

	// �͈͓����ǂ���
	if (IsMinus(dot1) && IsMinus(dot2) ||
		!IsMinus(dot1) && !IsMinus(dot2))
	{
		return false;
	}

	return true;
}

bool Collision::CheckAABB2AABB(const AABB &pos1, const AABB &pos2)
{
	// x, y, z���ɂ���
	if (pos1.near_right_downer.x > pos2.near_left_upper.x &&
		pos1.near_left_upper.x < pos2.near_right_downer.x &&
		pos1.near_left_upper.y > pos2.near_right_downer.y &&
		pos1.near_right_downer.y < pos2.near_left_upper.y &&
		pos1.far_left_upper.z > pos2.near_right_downer.z &&
		pos1.near_right_downer.z < pos2.far_left_upper.z)
	{
		return true;
	}

	return false;
}