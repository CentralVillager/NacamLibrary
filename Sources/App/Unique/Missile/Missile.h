#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "MissileDescs.h"

class Player;

/// <summary>
/// ミサイル
/// </summary>
class Missile : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

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

	inline static Player *p_player_;

public:

	Missile();
	~Missile();

public:

	static void SetPtr(Player *p)
	{
		p_player_ = p;
	}
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
	MissileParam &GetMissileParam() { return mi_param_; }

	void SetMissileLife(const int life) { mi_param_.life = life; }
	void SetTgtPos(const XMFLOAT3 &pos) { mi_param_.tgt_pos = pos; }

public:

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
	/// 敵の位置を算出する
	/// </summary>
	/// <param name="enemies"></param>
	/// <returns></returns>
	void UpdateTargetPos(EnemiesList &enemies);

	/// <summary>
	/// 敵を追尾する
	/// </summary>
	/// <param name="enemies">敵のコンテナ</param>
	void HomingEnemy(EnemiesList &enemies);

	void TestHomingTarget(EnemiesList &enemies);

	/// <summary>
	/// 設定した位置に追尾する
	/// </summary>
	void HomingTarget();

	/// <summary>
	/// エミッターの終了準備をする
	/// </summary>
	void PrepareTermEmitter();

private:

	/// <summary>
	/// エミッターに関する更新処理
	/// </summary>
	void UpdateEmitter();
};
