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

	// パーティクル管理コンテナ
	std::forward_list<Particle> particles_;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// パーティクルを生成する
	/// </summary>
	/// <param name="p">一粒の初期値</param>
	/// <param name="rand_range">位置のランダム幅</param>
	/// <param name="num_per_frame">生成する数 / f</param>
	void GenerateParticle(ParticleMember p, XMFLOAT3 rand_range, int num_per_frame);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ用描画
	/// </summary>
	void DebugDraw();

private:

	// コンテナに粒を追加
	void Add(ParticleMember p);

	// 値を生成する
	ParticleMember GenerateValue(ParticleMember p, XMFLOAT3 rand_range);
};
