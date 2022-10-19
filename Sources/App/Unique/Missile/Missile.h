#pragma once
#include "../Sources/App/Particle/Emitter.h"
#include "../Enemy/EnemiesList.h"
#include "../LockOnSystem/LockOnSystem.h"
#include "../Abs/AbsUniqueObj.h"

struct MissileArgs
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT3 acc;

	XMFLOAT3 tgt_pos;
	int tgt_id;

	float detection_range;
	UINT init_straight_time_;
	UINT life;
	bool is_validity;

	MissileArgs() :
		pos(XMFLOAT3(0, 0, 0)),
		vel(XMFLOAT3(0, 0, 0)),
		acc(XMFLOAT3(0, 0, 0)),
		tgt_pos(XMFLOAT3(0, 0, 0)),
		tgt_id(0),
		detection_range(0),
		init_straight_time_(0),
		life(0),
		is_validity(false)
	{}
};

class Missile : public AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	static std::unique_ptr<Model> model_;
	static std::unique_ptr<Model> coll_model_;

	// エミッター
	std::unique_ptr<Emitter> emitter_;
	MissileArgs mi_args_;
	float min_dist_;
	float rot_dead_zone_;

	UINT tgt_index_;

	LockOnSystem *lockon_sys_;

public:

	Missile();
	~Missile();

public:

	static void LoadResources();

	void Initialize(const MissileArgs &args, LockOnSystem *sys);
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
	void DrawColl() override;
	void DebugDraw() override;

public:

	const bool &GetIsValidity() { return mi_args_.is_validity; }

	void SetMissileLife(const int &life) { mi_args_.life = life; }
	void SetTgtPos(const XMFLOAT3 pos) { mi_args_.tgt_pos = pos; }

	void Activate();
	void InvalidateMissile() { mi_args_.is_validity = false; }

	void MoveZ(float speed);
	void HomingTarget(const XMFLOAT3 &target_pos);
	void HomingTarget(EnemiesList &enemies);
	void TermEmitter();

private:

	void UpdateEmitter();
};
