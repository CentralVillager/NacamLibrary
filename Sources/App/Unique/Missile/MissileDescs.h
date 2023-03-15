#pragma once

/*
	ミサイルに関する設定構造体などをまとめたヘッダです。
*/

/// <summary>
/// 発射方法
/// </summary>
enum class MissileType
{
	Charge,
	Ultimate,
	Mono,
	ForEnemy
};

/// <summary>
/// 追尾の精度
/// </summary>
enum class HomingAccuracy
{
	High,
	Middle,
	Low,
	NoHoming
};

/// <summary>
/// ミサイルの発射人
/// </summary>
enum class LaunchedBy
{
	Player,
	Enemy
};

/// <summary>
/// ターゲットとなりうるオブジェクトの種類
/// </summary>
enum class TargetIs
{
	Player = -2
};

/// <summary>
/// ミサイルが持つパラメータ
/// </summary>
struct MissileParam
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	MissileType type;			// ミサイルの発射方法

	float speed;				// 既定速度

	XMFLOAT3 pos;				// 位置
	XMFLOAT3 vel;				// 現在速度
	XMFLOAT3 acc;				// 加速度

	XMFLOAT3 tgt_pos;			// ターゲットの位置
	int32_t tgt_id;				// ターゲットのID

	float detection_range;		// ターゲット探知範囲
	UINT init_straight_time;	// 追尾を開始するまでの時間
	bool use_homing_time;		// 追尾時間を設けるか
	UINT homing_time;			// 追尾時間
	UINT life;					// 寿命
	bool is_validity;			// ミサイルが有効か
	bool is_explode;			// 爆発中か

	XMFLOAT3 trail_color;		// 軌跡の色

	HomingAccuracy accuracy_type;	// 追尾精度

	MissileParam() :
		type(),
		speed(),
		pos(),
		vel(),
		acc(),
		tgt_pos(),
		tgt_id(),
		detection_range(),
		init_straight_time(),
		use_homing_time(),
		homing_time(),
		life(),
		is_validity(),
		is_explode(),
		trail_color(),
		accuracy_type()
	{}
};
