#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"

/// <summary>
/// ミサイルが持つパラメータ
/// </summary>
struct MissileArgs
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 pos;				// 位置
	XMFLOAT3 vel;				// 速度
	XMFLOAT3 acc;				// 加速度

	XMFLOAT3 tgt_pos;			// ターゲットの位置
	int tgt_id;					// ターゲットのID

	float detection_range;		// ターゲット探知範囲
	UINT init_straight_time;	// 追尾を開始するまでの時間
	UINT life;					// 寿命
	bool is_validity;			// ミサイルが有効か

	MissileArgs() :
		pos(XMFLOAT3(0, 0, 0)),
		vel(XMFLOAT3(0, 0, 0)),
		acc(XMFLOAT3(0, 0, 0)),
		tgt_pos(XMFLOAT3(0, 0, 0)),
		tgt_id(0),
		detection_range(0),
		init_straight_time(0),
		life(0),
		is_validity(false)
	{}
};

class Missile : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	std::unique_ptr<Emitter> emitter_;	// エミッター
	MissileArgs mi_args_;				// パラメータ

public:

	Missile();
	~Missile();

public:

	static void LoadResources();

	void Initialize(const MissileArgs &args);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	const bool &GetIsValidity() { return mi_args_.is_validity; }

	void SetMissileLife(const int life) { mi_args_.life = life; }
	void SetTgtPos(const XMFLOAT3 &pos) { mi_args_.tgt_pos = pos; }

	/// <summary>
	/// 死亡フラグを含めてミサイルを有効化
	/// </summary>
	void Activate();

	/// <summary>
	/// 死亡フラグはそのままにミサイルを無効化
	/// </summary>
	void InvalidateMissile() { mi_args_.is_validity = false; }

	/// <summary>
	/// z軸方向へ進む
	/// </summary>
	void MoveZ(float speed);

	/// <summary>
	/// ターゲットを追尾する
	/// </summary>
	void HomingTarget(EnemiesList &enemies);

	void TestHomingTarget(EnemiesList &enemies);

	/// <summary>
	/// エミッターの終了準備をする
	/// </summary>
	void PrepareTermEmitter();

private:

	void UpdateEmitter();
};
