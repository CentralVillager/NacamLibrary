#pragma once
#pragma once
#include <array>
#include <list>
#include <DirectXMath.h>
#include "../../Lib/Line/Line.h"
#include "../../Lib/Camera/Camera.h"

class NcmGridFloor
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	static constexpr uint32_t LINE_NUM_ = 100;			// Line�̖{��
	static constexpr float Y_POS_ = -10.0f;				// y���W�̈ʒu
	static constexpr float GENE_POS_OFFSET_ = 500.0f;	// player�̈ʒu�ɑ΂��Ă̐��̐����ʒu
	static constexpr float DELETE_POS_OFFSET_ = 100.0f;	// player�̈ʒu�ɑ΂��Ă̐��̏����ʒu
	static constexpr float GENE_THRESHOLD_ = 10.0f;		// Line�̐���臒l

private:

	static Camera *cam_ptr_;	// �J�����̃|�C���^

private:

	std::list<Line> vertical_line_;		// �cLine
	std::list<Line> horizontal_line_;	// ��Line

	XMFLOAT3 center_pos_;		// ��ʒu
	float line_interval_;		// ���̊Ԋu
	float floor_size_;			// �t���A�̃T�C�Y

	float locate_point_for_auto_move_;	// Line�̔z�u�ʒu
	float vanish_point_for_auto_move_;	// Line�̏����ʒu

	XMFLOAT3 horizontal_left_;	// ���s���̍��ʒu
	XMFLOAT3 horizontal_right_;	// ���s���̉E�ʒu

	float gene_point_;			// Line�̐����ʒu
	float vanish_point_;		// Line�̏����ʒu

	float player_z_;			// player��z�ʒu

	float most_z_line_pos_;		// �ł����ɂ�����̈ʒu

public:

	NcmGridFloor();
	~NcmGridFloor();

public:

	static void SetCamera(Camera *cam) { cam_ptr_ = cam; }

public:

	void Initialize(XMFLOAT3 &center_pos, float interval);
	void Update();
	void Draw();
	void DebugDraw();

private:

	void InitVertLine();
	void InitHoriLine();

	/// <summary>
	/// �O���b�h����O�ɐi�܂���
	/// </summary>
	/// <param name="speed"></param>
	void MoveZ(float speed);

	/// <summary>
	/// �����𐶐�����
	/// </summary>
	void GenerateHoriLine();

	/// <summary>
	/// �������폜����
	/// </summary>
	void DeleteHoriLine();

	/// <summary>
	/// Line�̍ŉ��ʒu���Z�o����
	/// </summary>
	void CalcMostZPos();

public:

	/// <summary>
	/// �c����z�ʒu���v���C���[�ɒǏ]������
	/// </summary>
	void FollowVertLineForPlayer(const float player_z);
};
