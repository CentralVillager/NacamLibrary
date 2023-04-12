#pragma once
#include <memory>
#include <wrl.h>
#include <array>
#include "../AbsScene.h"
#include "../../../Lib/Collision/CollisionPrimitive.h"
#include "../../../Lib/Camera/Camera.h"
#include "../../../Lib/3D/Model.h"
#include "../../../Lib/3D/Object3d.h"
#include "../../../Lib/3D/Line.h"

//struct Vec2 {
//	float x;
//	float y;
//
//	Vec2() = default;
//
//	Vec2(float _x, float _y)
//		: x(_x)
//		, y(_y) {
//	}
//
//	float length() const	// ����
//	{
//		return sqrt(x * x + y * y);
//	}
//
//	float lengthSquare() const // �����̓��
//	{
//		return x * x + y * y;
//	}
//
//	float dot(const Vec2 &a) const // ����
//	{
//		return x * a.x + y * a.y;
//	}
//
//	float distance(const Vec2 &a) const    // ����
//	{
//		return sqrt((a.x - x) * (a.x - x) + (a.y - y) * (a.y - y));
//	}
//
//	Vec2 normalized() const // ���K��
//	{
//		float	_len = length();
//		return{ x / _len , y / _len };
//	}
//
//	bool isZero() const // �[���x�N�g���ł��邩
//	{
//		return x == 0.0 && y == 0.0;
//	}
//
//	void clear(float a) {
//		x = a;
//		y = a;
//	}
//
//	Vec2 operator +() const {
//		return *this;
//	}
//
//	Vec2 operator -() const {
//		return{ -x, -y };
//	}
//
//	Vec2 operator +(const Vec2 &other) const {
//		return{ x + other.x, y + other.y };
//	}
//
//	Vec2 operator -(const Vec2 &other) const {
//		return{ x - other.x, y - other.y };
//	}
//
//	Vec2 operator *(float s) const {
//		return{ x * s, y * s };
//	}
//
//	Vec2 operator /(float s) const {
//		return{ x / s, y / s };
//	}
//
//	Vec2 &operator +=(const Vec2 &other) {
//		x += other.x;
//		y += other.y;
//		return *this;
//	}
//
//	Vec2 &operator -=(const Vec2 &other) {
//		x -= other.x;
//		y -= other.y;
//		return *this;
//	}
//
//	Vec2 &operator *=(float s) {
//		x *= s;
//		y *= s;
//		return *this;
//	}
//
//	Vec2 &operator /=(float s) {
//		x /= s;
//		y /= s;
//		return *this;
//	}
//};
//
//inline Vec2 operator *(float s, const Vec2 &v) // Vec2 ����ɂ��� 2�� *
//{
//	return{ s * v.x, s * v.y };
//}
//
//class Obj {
//
//public:
//
//	static const float RADIUS_;			// ���̂̔��a
//	static const float LENGTH_LINK_;	// ���̂̃����N�̒���
//	static const float K_HOOK_;			// �t�b�N�̖@���̌W��K
//	static const float K_V_RES_;		// �S����R�W��
//	static const float ACC_G_;			// �d�͉����x
//	static const int NUM_HIST_ = 1000;
//
//	int	stat;		//0=���݂��Ȃ��A1=���݂��Ċ������A2=���݂��ĐÎ~
//	int	graph;
//
//	Vec2	pos;
//	Vec2	size;
//	Vec2	vel;
//	Vec2	acc;
//	float	m;
//	float	r;
//	float	dist;
//	Vec2	hp[NUM_HIST_];
//	int		idx_hist;
//	int	idx;
//	Obj *link0;		//�S���łȂ����Ă���OBJ
//	Obj *link1;		//�S���łȂ����Ă���OBJ
//	int	grabbed;	//�}�E�X�Œ͂܂�Ă���
//
//	void update();
//	void disp();
//
//	Obj() {
//		pos.clear(0);
//		r = RADIUS_;
//		dist = 0;
//		vel.clear(0);
//		acc.clear(0);
//		m = 1.0f;
//		stat = 0;
//		idx_hist = 0;
//		for (int i = 0; i < NUM_HIST_; i++) {
//			hp[i].clear(0);
//		}
//		link0 = NULL;
//		link1 = NULL;
//		grabbed = 0;
//	}
//
//};

class MathDemoScene : public AbsScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

public:

	const float PI_ = 3.14159265f;
	const float LENGTH_ = 10.0f;
	const float CLENGTH_ = (LENGTH_ * 2 * PI_);
	const float MASS_ = 0.005f;
	//const float MASS_ = 0.1346f;
	const float G_ = -9.8f;

	float x;     // �R��L�΂��Ĉ���������ꍇ�ɏo����~����̍��W�A�O�͕R�����̐^���ɂ���ʒu
	float speed; // x�̑��x
	float angle;
	float jiku_x = 0, jiku_y = 0; // ���̈ʒu
	float nx, ny;

private:

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���f��
	std::unique_ptr<Model> model_;

	// �I�u�W�F�N�g
	std::array<Object3d, 2> object_;

	//std::array<Obj, 3> obj_;

public:

	MathDemoScene();
	~MathDemoScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void AfterPostEffectDraw() override;
	void DebugDraw() override;
};
