#pragma once
#include "Particle.h"
#include <forward_list>
#include <list>
#include <memory>
#include <DirectXMath.h>
#include "../Sources/Lib/Model/Model.h"
#include "../../Lib/Sprite/NcmSprite.h"
#include "NcmParticleCommonArgs.h"

// エミッターの設定構造体
struct EmitterDesc
{
	ParticleDesc part_desc_;		// パーティクルの構成要素
	DirectX::XMFLOAT3 pos_rand_;	// 生成位置の乱数の振れ幅
	DirectX::XMFLOAT3 vel_rand_;	// 速度の乱数の振れ幅
	UINT gene_num_;					// 生成する個数
	bool use_life_;					// エミッターの寿命を有効にするか
	UINT life_;						// エミッターの寿命
	bool is_dead_;					// 死んでいるか
	ncm_thandle tex_handle_;		// テクスチャハンドル

	EmitterDesc() :
		part_desc_(),
		pos_rand_(),
		vel_rand_(),
		gene_num_(),
		use_life_(),
		life_(1),
		is_dead_(),
		tex_handle_()
	{}
};

class Emitter
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// あらかじめ確保する数
	static constexpr uint32_t RESERVE_NUM_ = 512;

private:

	// モデルデータ
	static std::unique_ptr<Model> model_;

	// テクスチャハンドル
	static ncm_thandle tex_handle_;

	// リソースシェア版パーティクル管理コンテナ
	static std::list<Particle> shared_particles_;

	// 使用されたパーティクルの数
	static uint32_t particle_num_;

private:

	// パーティクル管理コンテナ
	std::forward_list<Particle> particles_;

	// パーティクル生成に必要な要素
	EmitterDesc emitter_desc_;

	// パーティクルが使用されたかに関するダーティフラグ
	bool updated_dirty_ = false;

public:

	// アクセッサ
	inline const EmitterDesc &GetEmitterDesc() { return emitter_desc_; }
	inline const XMFLOAT3 &GetPosition() { return emitter_desc_.part_desc_.position_; }

	inline void SetEmitterDesc(const EmitterDesc &p) { emitter_desc_ = p; }
	inline void SetPosition(const XMFLOAT3 &pos) { emitter_desc_.part_desc_.position_ = pos; }

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void LoadResources();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 確保されたパーティクルを使う
	/// </summary>
	void UseParticle(std::list<Particle>::iterator last_itr);

	/// <summary>
	/// パーティクルを生成する
	/// </summary>
	void GenerateParticle();

	/// <summary>
	/// パーティクルを更新する
	/// </summary>
	void UpdateParticle();

	/// <summary>
	/// エミッター終了準備をする
	/// </summary>
	void PrepareTerminate();

	/// <summary>
	/// エミッターが終了出来るかを知らせる
	/// </summary>
	/// <returns>可否</returns>
	bool NoticeCanTerminate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ用描画
	void DebugDraw();
	static void StaticDebugDraw();

private:

	// コンテナに粒を追加
	void Add(const ParticleDesc &p);

	// 値を生成する
	ParticleDesc GenerateValue(const EmitterDesc &emitter);
};