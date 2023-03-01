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

	static constexpr uint32_t LINE_NUM_ = 100;			// Lineの本数
	static constexpr float Y_POS_ = -10.0f;				// y座標の位置
	static constexpr float GENE_POS_OFFSET_ = 500.0f;	// playerの位置に対しての線の生成位置
	static constexpr float DELETE_POS_OFFSET_ = 100.0f;	// playerの位置に対しての線の消去位置
	static constexpr float GENE_THRESHOLD_ = 10.0f;		// Lineの生成閾値

private:

	static Camera *cam_ptr_;	// カメラのポインタ

private:

	std::list<Line> vertical_line_;		// 縦Line
	std::list<Line> horizontal_line_;	// 横Line

	XMFLOAT3 center_pos_;		// 基準位置
	float line_interval_;		// 線の間隔
	float floor_size_;			// フロアのサイズ

	float locate_point_for_auto_move_;	// Lineの配置位置
	float vanish_point_for_auto_move_;	// Lineの消去位置

	XMFLOAT3 horizontal_left_;	// 平行線の左位置
	XMFLOAT3 horizontal_right_;	// 平行線の右位置

	float gene_point_;			// Lineの生成位置
	float vanish_point_;		// Lineの消去位置

	float player_z_;			// playerのz位置

	float most_z_line_pos_;		// 最も奥にある線の位置

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
	/// グリッドを手前に進ませる
	/// </summary>
	/// <param name="speed"></param>
	void MoveZ(float speed);

	/// <summary>
	/// 横線を生成する
	/// </summary>
	void GenerateHoriLine();

	/// <summary>
	/// 横線を削除する
	/// </summary>
	void DeleteHoriLine();

	/// <summary>
	/// Lineの最奥位置を算出する
	/// </summary>
	void CalcMostZPos();

public:

	/// <summary>
	/// 縦線のz位置をプレイヤーに追従させる
	/// </summary>
	void FollowVertLineForPlayer(const float player_z);
};
