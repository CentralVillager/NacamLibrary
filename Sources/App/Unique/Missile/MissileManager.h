#pragma once
#include <vector>
#include <list>
#include <DirectXMath.h>
#include "Missile.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../../Collision/CollisionPrimitive.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"

class Player;

/// <summary>
/// ミサイルのコンテナを管理します。
/// </summary>
class MissileManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:

	// ゲーム内に存在する全てのミサイルを管理するコンテナ
	std::list<Missile> missile_list_;

	Player *p_player_;
	LockOnSystem *p_lockon_sys_;

public:

	MissileManager();
	~MissileManager();

public:

	void Initialize(Player *p, LockOnSystem *sys);
	void Finalize();
	void Update();
	void Draw();
	void DrawColl();
	void DebugDraw();

public:

	inline const std::list<Missile> &GetMissileList() { return missile_list_; }
	const bool &GetIsValidity(UINT n);
	const Sphere &GetCollData(UINT n);
	Player *GetPlayerPtr() { return p_player_; }
	LockOnSystem *GetLockOnSys() { return p_lockon_sys_; }

public:

	/// <summary>
	/// 毎フレーム敵追尾計算
	/// </summary>
	/// <param name="enemies"></param>
	void HomingEnemy(EnemiesList &enemies);

	/// <summary>
	/// 毎フレームターゲット追尾計算
	/// </summary>
	void HomingTarget();

	/// <summary>
	/// 毎フレームターゲット位置更新
	/// </summary>
	/// <param name="enemies"></param>
	void UpdateTargetPos(EnemiesList &enemies);

	/// <summary>
	/// 死亡処理
	/// </summary>
	/// <param name="n"></param>
	bool Death(UINT n);

	/// <summary>
	/// 新たにミサイルを生成し追加する
	/// </summary>
	/// <param name="param"></param>
	void AddMissile(const MissileParam &param);
};
