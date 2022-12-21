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
/// �~�T�C���̃R���e�i���Ǘ����܂��B
/// </summary>
class MissileManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:

	// �Q�[�����ɑ��݂���S�Ẵ~�T�C�����Ǘ�����R���e�i
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
	/// ���t���[���G�ǔ��v�Z
	/// </summary>
	/// <param name="enemies"></param>
	void HomingEnemy(EnemiesList &enemies);

	/// <summary>
	/// ���t���[���^�[�Q�b�g�ǔ��v�Z
	/// </summary>
	void HomingTarget();

	/// <summary>
	/// ���t���[���^�[�Q�b�g�ʒu�X�V
	/// </summary>
	/// <param name="enemies"></param>
	void UpdateTargetPos(EnemiesList &enemies);

	/// <summary>
	/// ���S����
	/// </summary>
	/// <param name="n"></param>
	bool Death(UINT n);

	/// <summary>
	/// �V���Ƀ~�T�C���𐶐����ǉ�����
	/// </summary>
	/// <param name="param"></param>
	void AddMissile(const MissileParam &param);
};
