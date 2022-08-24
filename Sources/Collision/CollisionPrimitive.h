#pragma once
#include <DirectXMath.h>

/// <summary>
/// ��
/// </summary>
struct Sphere
{
	// ���S���W
	DirectX::XMVECTOR center = { 0, 0, 0, 1 };

	// ���a
	float radius = 1.0f;
};

/// <summary>
/// ����
/// </summary>
struct Plane
{
	// �@���x�N�g��
	DirectX::XMVECTOR normal = { 0, 1, 0, 0 };

	// ���_(0, 0, 0)����̋���
	float distance = 0.0f;
};

/// <summary>
/// �O�p�`
/// </summary>
class Triangle
{
public:

	// ���_���W3��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;

	// �@���x�N�g��
	DirectX::XMVECTOR normal;

	/// <summary>
	/// �@���̌v�Z
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// ���C�i�������j
/// </summary>
struct Ray
{
	// �n�_���W
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };

	// ����
	DirectX::XMVECTOR dir = { 1, 0 ,0 ,0 };
};

/// <summary>
/// ����
/// </summary>
struct LineSegment
{
	// �n�_���W
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };

	// �I�_���W
	DirectX::XMVECTOR end = { 0, 0, 0, 1 };
};

/// <summary>
/// �����s�o�E���f�B���O�{�b�N�X
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