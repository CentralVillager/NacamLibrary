#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"

/// <summary>
/// ミサイルが持つパラメータ
/// </summary>
struct MissileParam
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
	bool is_explode;			// 爆発中か

	MissileParam() :
		pos(),
		vel(),
		acc(),
		tgt_pos(),
		tgt_id(),
		detection_range(),
		init_straight_time(),
		life(),
		is_validity(),
		is_explode()
	{}
};

/// <summary>
/// ミサイル
/// </summary>
class Missile : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:

	/// <summary>
	/// ターゲット検索結果
	/// </summary>
	struct SearchResult
	{
		bool is_succes;
		XMFLOAT3 pos;

		SearchResult() :
			is_succes(),
			pos()
		{}
	};

private:

	// 爆発パーティクルの粒の数目安
	static constexpr uint32_t EXPLO_PARTICLE_NUM_ = 100;

private:

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

private:

	std::unique_ptr<Emitter> emitter_;	// エミッター
	std::unique_ptr<Emitter> explo_emi_;// 爆発エミッター
	MissileParam mi_param_;				// パラメータ

public:

	Missile();
	~Missile();

public:

	static void LoadResources();

	void Initialize(const MissileParam &args);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	const bool &GetIsValidity() { return mi_param_.is_validity; }

	void SetMissileLife(const int life) { mi_param_.life = life; }
	void SetTgtPos(const XMFLOAT3 &pos) { mi_param_.tgt_pos = pos; }

	/// <summary>
	/// 死亡フラグを含めてミサイルを有効化
	/// </summary>
	void Activate();

	/// <summary>
	/// 死亡フラグはそのままにミサイルを無効化
	/// </summary>
	void InvalidateMissile();

	/// <summary>
	/// 死亡時の爆発
	/// </summary>
	void ExplosionOnDeath();

	/// <summary>
	/// z軸方向へ進む
	/// </summary>
	void MoveZ(float speed);

	/// <summary>
	/// ターゲットの位置を算出する
	/// </summary>
	/// <param name="enemies"></param>
	/// <returns></returns>
	SearchResult CalcEnemyPos(EnemiesList &enemies);

	/// <summary>
	/// 敵を追尾する
	/// </summary>
	/// <param name="enemies">敵のコンテナ</param>
	void HomingEnemy(EnemiesList &enemies);

	void TestHomingTarget(EnemiesList &enemies);

	/// <summary>
	/// 指定した位置に追尾する
	/// </summary>
	/// <param name="target"></param>
	void HomingTarget(XMFLOAT3 &target);

	/// <summary>
	/// エミッターの終了準備をする
	/// </summary>
	void PrepareTermEmitter();

private:

	void UpdateEmitter();
};
