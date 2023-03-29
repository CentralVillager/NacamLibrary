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

	// サイズを計算
	floor_size_ = LINE_NUM_ * line_interval_;

	Line::SetCamera(cam_ptr_);

	// 線コンテナを初期化
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

	// 最初に上から見た場合の左下位置を求める
	front_pos.x = center_pos_.x - (floor_size_ / 2);
	front_pos.y = Y_POS_;
	front_pos.z = center_pos_.z - (floor_size_ / 2);

	// 消去位置を算出
	vanish_point_for_auto_move_ = 100;

	// 最初に上から見た場合の左上位置を求める
	back_pos.x = front_pos.x;
	back_pos.y = front_pos.y;
	back_pos.z = front_pos.z + floor_size_;

	// 配置位置を算出
	locate_point_for_auto_move_ = 110;

	// 全てのLineに対して
	for (auto &i : vertical_line_)
	{
		// 頂点位置を設定
		i.SetVertPos(front_pos, (UINT)(Start));
		i.SetVertPos(back_pos, (UINT)(End));

		// 次の頂点位置を算出
		front_pos.x += line_interval_;
		back_pos.x += line_interval_;
	}
}

void NcmGridFloor::InitHoriLine()
{
	using enum VertDesc;

	// 最初に上から見た場合の左下位置を求める
	horizontal_left_.x = center_pos_.x - (floor_size_ / 2);
	horizontal_left_.y = Y_POS_;
	horizontal_left_.z = center_pos_.z - (floor_size_ / 2);

	// 最初に上から見た場合の右下位置を求める
	horizontal_right_.x = horizontal_left_.x + floor_size_;
	horizontal_right_.y = horizontal_left_.y;
	horizontal_right_.z = horizontal_left_.z;

	// 全てのLineに対して
	//for (auto &i : horizontal_line_)
	//{
	//	// 頂点位置を設定
	//	i.SetVertPos(horizontal_left_, (UINT)(Start));
	//	i.SetVertPos(horizontal_right_, (UINT)(End));

	//	// 次の頂点位置を算出
	//	horizontal_left_.z += line_interval_;
	//	horizontal_right_.z += line_interval_;
	//}

	for (int i = 0; i < LINE_NUM_; i++)
	{
		horizontal_line_.emplace_back();
		horizontal_line_.back().Initialize();
		horizontal_line_.back().SetVertPos(horizontal_left_, (UINT)(Start));
		horizontal_line_.back().SetVertPos(horizontal_right_, (UINT)(End));

		// 次の頂点位置を算出
		horizontal_left_.z += line_interval_;
		horizontal_right_.z += line_interval_;
	}

	// 最奥の線の位置を取得
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

		// 消去位置を過ぎたら
		if (pos_z <= vanish_point_for_auto_move_)
		{
			// z位置を既定の配置位置へ
			i.SetPosition(XMFLOAT3(0, 0, locate_point_for_auto_move_));
		}
	}
}

void NcmGridFloor::GenerateHoriLine()
{
	using enum VertDesc;

	// 生成位置 - 最奥の線位置 >= 線の描画間隔
	if ((gene_point_ - most_z_line_pos_) >= line_interval_)
	{
		float gene_pos = gene_point_;
		//float gene_pos = most_z_line_pos_ + line_interval_;

		// 新規線の生成
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

	// 消失点を過ぎた線を削除する
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
	// プレイヤーの位置を更新
	player_z_ = player_z;
	// 線の生成位置を更新
	gene_point_ = player_z_ + GENE_POS_OFFSET_;
	// 線の消去位置を更新
	vanish_point_ = player_z_ - DELETE_POS_OFFSET_;

	// 全ての垂直線に対して
	for (auto &i : vertical_line_)
	{
		// 位置を更新
		i.SetPosition(XMFLOAT3(0, 0, player_z_));
	}
}
