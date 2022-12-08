#pragma once
#include <memory>
#include <forward_list>
#include "../../App/Particle/Emitter.h"
#include "../../Lib/PlatePoly/NcmPlatePoly.h"
#include "NcmParticleCommonArgs.h"

/// <summary>
/// 板ポリを使用したパーティクルのマネージャ
/// </summary>
class NcmParticleManager
{
	static std::forward_list<NcmParticleCommonArgs> part_args_;	// 描画用共通データ
	static size_t particle_num;
	static size_t max_particle_num;

	std::unique_ptr<NcmPlatePoly> poly_;	// 描画データ

public:

	NcmParticleManager();
	~NcmParticleManager();

public:

	/// <summary>
	/// 新たにパーティクルの要素を追加
	/// </summary>
	/// <param name="args"></param>
	static void AddParticleCommonArgs(std::forward_list<NcmParticleCommonArgs> &args);

	/// <summary>
	/// 格納されていた要素を全削除
	/// </summary>
	static void ClearParticleArgsBeforeUpdate();

public:

	void Initialize();
	void Update();
	void Draw();
	static void StaticDebugDraw();
};
