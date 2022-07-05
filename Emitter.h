#pragma once
#include "Particle.h"
#include <forward_list>
#include "Model.h"
#include <memory>
#include <DirectXMath.h>

class Emitter {
	using XMFLOAT3 = DirectX::XMFLOAT3;

	// モデルデータ
	static std::unique_ptr<Model> model_;

public:

	// エミッター生成に必要な要素
	struct EmitterArgs {

		// パーティクルの構成要素
		ParticleMember particle;

		// 生成位置の乱数の振れ幅
		XMFLOAT3 pos_rand_;

		// 速度の乱数の振れ幅
		XMFLOAT3 vel_rand_;

		// 生成する個数
		UINT gene_num_;

		// エミッターの寿命を有効にするか
		bool use_life_;

		// エミッターの寿命
		UINT life_ = 1;

		bool is_dead_ = false;
	};

private:

	// パーティクル管理コンテナ
	std::forward_list<Particle> particles_;

	// パーティクル生成に必要な要素
	EmitterArgs emitter_args_;

public:

	// アクセッサ
	inline const EmitterArgs &GetEmitterArgs() { return emitter_args_; }
	inline const XMFLOAT3 &GetPosition() { return emitter_args_.particle.position_; }

	inline void SetEmitterArgs(const EmitterArgs &p) { emitter_args_ = p; }
	inline void SetPosition(const XMFLOAT3 &pos) { emitter_args_.particle.position_ = pos; }

private:

	// コンテナに粒を追加
	void Add(ParticleMember p);

	// 値を生成する
	ParticleMember GenerateValue(EmitterArgs emitter);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// パーティクルを生成する
	/// </summary>
	void GenerateParticle();

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
	/// </summary>
	void DebugDraw(const string &name);
	void DebugDraw();
};