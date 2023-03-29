#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"
#include "MissileDescs.h"
#include "MissileHoming.h"
#include "../../Lib/Math/Easing/NcmEasing.h"

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

	inline static Player *p_player_;

private:

	// 追尾精度
	std::unique_ptr<MissileHoming> homing_sequence_;

	std::unique_ptr<Emitter> emitter_;		// エミッター
	std::unique_ptr<Emitter> explo_emi_;	// 爆発エミッター
	MissileParam mi_param_;					// パラメータ

	ncm_ehandle for_lerp_handle_x_;			// 補間用イージングハンドル
	ncm_ehandle for_lerp_handle_y_;			// 補間用イージングハンドル
	ncm_ehandle for_lerp_handle_z_;			// 補間用イージングハンドル

	// パーティクル補間用
	XMFLOAT3 before_particle_pos_;
	XMFLOAT3 current_particle_pos_;

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
	/// <param name="enemies"></param>emitter
	void UpdateTargetPos(EnemiesList &enemies);

	/// <summary>
	/// 設定した位置に追尾する
	/// </summary>
	void HomingTarget(HomingAccuracy accuracy);

	/// <summary>
	/// エミッターの終了準備をする
	/// </summary>
	void PrepareTermEmitter();

private:

	/// <summary>
	/// エミッターに関する更新処理
	/// </summary>
	void UpdateEmitter();

	/// <summary>
	/// ミサイルパーティクルの補間処理
	/// </summary>
	void InterpolateParticle();

	/// <summary>
	/// XMFLOAT3型の全てのメンバが0かの判定
	/// </summary>
	/// <param name="src">判定素材</param>
	/// <returns>全て0ならtrue</returns>
	inline bool IsAllZero(XMFLOAT3 &src)
	{
		if (src.x == 0.0f && src.y == 0.0f && src.z == 0.0f)
		{
			return true;
		}

		return false;
	}
};

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3 &lhs, const float &rhs);
