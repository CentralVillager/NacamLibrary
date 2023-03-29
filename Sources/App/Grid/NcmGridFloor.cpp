#include "NcmGridFloor.h"
#include "../../Lib/DebugUtill/NcmImGui.h"

Camera *NcmGridFloor::cam_ptr_ = nullptr;

NcmGridFloor::NcmGridFloor() :
	vertical_line_(),
	horizontal_line_(),
	center_pos_(),
	line_interval_(),
	floor_size_(),
	locate_point_for_auto_move_(),
	vanish_point_for_auto_move_(),
	horizontal_left_(),
	horizontal_right_(),
	gene_point_(),
	vanish_point_(),
	player_z_(),
	most_z_line_pos_()
{}

NcmGridFloor::~NcmGridFloor()
{}

void NcmGridFloor::Initialize(XMFLOAT3 &center_pos, float interval)
{
	center_pos_ = center_pos;
	line_interval_ = interval;

	// �T�C�Y���v�Z
	floor_size_ = LINE_NUM_ * line_interval_;

	Line::SetCamera(cam_ptr_);

	// ���R���e�i��������
	vertical_line_.resize(LINE_NUM_);
	//horizontal_line_.resize(LINE_NUM_);

	for (auto &i : vertical_line_)
	{
		i.Initialize();
	}

	for (auto &i : horizontal_line_)
	{
		//i.Initialize();
	}

	InitVertLine();
	InitHoriLine();
}

void NcmGridFloor::Update()
{
	for (auto &i : vertical_line_)
	{
		i.Update();
	}

	for (auto &i : horizontal_line_)
	{
		i.Update();
	}

	CalcMostZPos();
	GenerateHoriLine();
	DeleteHoriLine();
}

void NcmGridFloor::Draw()
{
	for (auto &i : vertical_line_)
	{
		i.Draw();
	}

	for (auto &i : horizontal_line_)
	{
		i.Draw();
	}
}

void NcmGridFloor::DebugDraw()
{
	ImGui::Text("player_z : %f", player_z_);
	ImGui::Text("most_z : %f", most_z_line_pos_);
	ImGui::Text("locate : %f", gene_point_);
	ImGui::Text("vanish : %f", vanish_point_);

	if (!horizontal_line_.empty())
	{
		ImGui::Text("near : %f", horizontal_line_.front().GetVertData()[0].pos.z);
		ImGui::Text("far : %f", horizontal_line_.back().GetVertData()[0].pos.z);
	}

	ImGui::Text("list");

	for (auto &i : horizontal_line_)
	{
		ImGui::Text("vert z : %f", i.GetVertData()[0].pos.z);
		ImGui::Text("pos z : %f", i.GetPosition().z);
	}
}

void NcmGridFloor::InitVertLine()
{
	using enum VertDesc;

	XMFLOAT3 front_pos{};
	XMFLOAT3 back_pos{};

	// �ŏ��ɏォ�猩���ꍇ�̍����ʒu�����߂�
	front_pos.x = center_pos_.x - (floor_size_ / 2);
	front_pos.y = Y_POS_;
	front_pos.z = center_pos_.z - (floor_size_ / 2);

	// �����ʒu���Z�o
	vanish_point_for_auto_move_ = 100;

	// �ŏ��ɏォ�猩���ꍇ�̍���ʒu�����߂�
	back_pos.x = front_pos.x;
	back_pos.y = front_pos.y;
	back_pos.z = front_pos.z + floor_size_;

	// �z�u�ʒu���Z�o
	locate_point_for_auto_move_ = 110;

	// �S�Ă�Line�ɑ΂���
	for (auto &i : vertical_line_)
	{
		// ���_�ʒu��ݒ�
		i.SetVertPos(front_pos, (UINT)(Start));
		i.SetVertPos(back_pos, (UINT)(End));

		// ���̒��_�ʒu���Z�o
		front_pos.x += line_interval_;
		back_pos.x += line_interval_;
	}
}

void NcmGridFloor::InitHoriLine()
{
	using enum VertDesc;

	// �ŏ��ɏォ�猩���ꍇ�̍����ʒu�����߂�
	horizontal_left_.x = center_pos_.x - (floor_size_ / 2);
	horizontal_left_.y = Y_POS_;
	horizontal_left_.z = center_pos_.z - (floor_size_ / 2);

	// �ŏ��ɏォ�猩���ꍇ�̉E���ʒu�����߂�
	horizontal_right_.x = horizontal_left_.x + floor_size_;
	horizontal_right_.y = horizontal_left_.y;
	horizontal_right_.z = horizontal_left_.z;

	// �S�Ă�Line�ɑ΂���
	//for (auto &i : horizontal_line_)
	//{
	//	// ���_�ʒu��ݒ�
	//	i.SetVertPos(horizontal_left_, (UINT)(Start));
	//	i.SetVertPos(horizontal_right_, (UINT)(End));

	//	// ���̒��_�ʒu���Z�o
	//	horizontal_left_.z += line_interval_;
	//	horizontal_right_.z += line_interval_;
	//}

	for (int i = 0; i < LINE_NUM_; i++)
	{
		horizontal_line_.emplace_back();
		horizontal_line_.back().Initialize();
		horizontal_line_.back().SetVertPos(horizontal_left_, (UINT)(Start));
		horizontal_line_.back().SetVertPos(horizontal_right_, (UINT)(End));

		// ���̒��_�ʒu���Z�o
		horizontal_left_.z += line_interval_;
		horizontal_right_.z += line_interval_;
	}

	// �ŉ��̐��̈ʒu���擾
	most_z_line_pos_ = horizontal_line_.back().GetVertData()[0].pos.z;
	gene_point_ = most_z_line_pos_ + line_interval_;
}

void NcmGridFloor::MoveZ(float speed)
{
	using enum VertDesc;

	for (auto &i : horizontal_line_)
	{
		float pos_z = i.GetPosition().z;
		pos_z -= speed;
		i.SetPosition(XMFLOAT3(0, 0, pos_z));

		// �����ʒu���߂�����
		if (pos_z <= vanish_point_for_auto_move_)
		{
			// z�ʒu������̔z�u�ʒu��
			i.SetPosition(XMFLOAT3(0, 0, locate_point_for_auto_move_));
		}
	}
}

void NcmGridFloor::GenerateHoriLine()
{
	using enum VertDesc;

	// �����ʒu - �ŉ��̐��ʒu >= ���̕`��Ԋu
	if ((gene_point_ - most_z_line_pos_) >= line_interval_)
	{
		float gene_pos = gene_point_;
		//float gene_pos = most_z_line_pos_ + line_interval_;

		// �V�K���̐���
		horizontal_line_.emplace_back();
		horizontal_line_.back().Initialize();
		horizontal_line_.back().SetVertPos(XMFLOAT3(horizontal_left_.x, horizontal_left_.y, gene_pos), (UINT)(Start));
		horizontal_line_.back().SetVertPos(XMFLOAT3(horizontal_right_.x, horizontal_right_.y, gene_pos), (UINT)(End));

		//most_z_line_pos_ = gene_pos;
	}
}

void NcmGridFloor::DeleteHoriLine()
{
	if (horizontal_line_.empty())
	{
		return;
	}

	// �����_���߂��������폜����
	if (horizontal_line_.begin()->GetVertData()[0].pos.z <= vanish_point_)
	{
		horizontal_line_.erase(horizontal_line_.begin());
	}
}

void NcmGridFloor::CalcMostZPos()
{
	most_z_line_pos_ = horizontal_line_.back().GetVertData()[0].pos.z;
}

void NcmGridFloor::FollowVertLineForPlayer(const float player_z)
{
	// �v���C���[�̈ʒu���X�V
	player_z_ = player_z;
	// ���̐����ʒu���X�V
	gene_point_ = player_z_ + GENE_POS_OFFSET_;
	// ���̏����ʒu���X�V
	vanish_point_ = player_z_ - DELETE_POS_OFFSET_;

	// �S�Ă̐������ɑ΂���
	for (auto &i : vertical_line_)
	{
		// �ʒu���X�V
		i.SetPosition(XMFLOAT3(0, 0, player_z_));
	}
}
