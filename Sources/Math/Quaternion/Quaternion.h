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
///// �������w�肵�ăN�H�[�^�j�I�����쐬
///// </summary>
///// <param name="x"></param>
///// <param name="y"></param>
///// <param name="z"></param>
///// <param name="w"></param>
///// <returns></returns>
//Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
//
///// <summary>
///// �C�ӎ��܂��̉�]�̎w�肩��N�H�[�^�j�I�����쐬
///// </summary>
///// <param name="v"></param>
///// <param name="angle"></param>
///// <returns></returns>
//Quaternion quaternion(const DirectX::XMFLOAT3 &v, float angle);
//
///// <summary>
///// �x�N�g�� v �� �p����\���N�H�[�^�j�I�� q �ŉ�]����
///// </summary>
///// <param name="v"></param>
///// <param name="q"></param>
///// <returns></returns>
//Quaternion quaternion(const DirectX::XMFLOAT3 &v, const Quaternion &q);
//
///// <summary>
///// ���ς����߂�
///// </summary>
///// <param name="q1">�l�P</param>
///// <param name="q2">�l�Q</param>
///// <returns>�P�ƂQ�̓���</returns>
//float Dot(const Quaternion &q1, const Quaternion &q2);
//
///// <summary>
///// �m���������߂�
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//float Length(const Quaternion &q);
//
///// <summary>
///// ���K������
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//Quaternion Normalize(const Quaternion &q);
//
///// <summary>
///// �����l���������߂�
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//Quaternion Conjugate(const Quaternion &q);
//
///// <summary>
///// �N�H�[�^�j�I�������ʐ��`��Ԃ���
///// </summary>
///// <param name="q1">��ԊJ�n�n�_</param>
///// <param name="p2">��ԏI���n�_</param>
///// <param name="t">����</param>
///// <returns>��Ԍ���</returns>
//Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, float t);
//
///// <summary>
///// �N�H�[�^�j�I������`��Ԃ���
///// </summary>
///// <param name="q1">��ԊJ�n�n�_</param>
///// <param name="q2">��ԏI���n�_</param>
///// <param name="t">����</param>
///// <returns><��Ԍ���/returns>
//Quaternion Lerp(const Quaternion &q1, const Quaternion &q2, float t);
//
///// <summary>
///// �N�H�[�^�j�I�������]�s��ɕϊ�����
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//DirectX::XMMATRIX Rotate(const Quaternion &q);
//
///// <summary>
///// ��]������N�H�[�^�j�I�������߂�
///// </summary>
///// <param name="m"></param>
///// <returns></returns>
//Quaternion quaternion(const DirectX::XMMATRIX &m);
//
///// <summary>
///// ��]���̎Z�o
///// </summary>
///// <param name="q"></param>
///// <returns></returns>
//DirectX::XMFLOAT3 GetAxis(const Quaternion &q);
//
//// �P�����Z�q�I�[�o�[���[�h
//Quaternion operator + (const Quaternion &q);
//Quaternion operator - (const Quaternion &q);
//
//// ������Z�q�I�[�o�[���[�h
//Quaternion &operator += (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator -= (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator *= (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator *= (Quaternion &q, float s);
//Quaternion &operator /= (Quaternion &q, float s);
//
//// 2�����Z�q�I�[�o�[���[�h
//Quaternion operator + (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator - (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator * (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator * (const Quaternion &q, float s);
//Quaternion operator * (float s, const Quaternion &q);
//Quaternion operator / (const Quaternion &q, float s);
