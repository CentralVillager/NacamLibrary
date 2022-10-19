#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../../../Lib/Object3d/Object3d.h"
#include "../../../Lib/Model/Model.h"
#include "../../../App/Collision/CollisionPrimitive.h"
#include "../../Debug/NcmImGui.h"

class AbsUniqueObj
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

protected:

	// 描画データ
	static std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> obj_;

	// 当たり判定描画データ
	static std::unique_ptr<Model> coll_model_;
	std::unique_ptr<Object3d> coll_obj_;
	Sphere coll_;
	float COLL_RADIUS_;

	XMFLOAT3 pos_;
	float speed_;
	bool is_dead_;

public:

	AbsUniqueObj(XMFLOAT3 pos, float speed);
	virtual ~AbsUniqueObj() = default;

public:

	inline const XMFLOAT3 &GetPos() { return pos_; }
	inline const Sphere &GetCollData() { return coll_; }
	inline const bool &IsDead() { return is_dead_ == true; }
	inline const void Death() { is_dead_ = true; }
	inline void SetPos(const XMFLOAT3 &pos) { pos_ = pos; }
	inline void SetIsDead(bool b) { is_dead_ = b; }

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawColl() = 0;
	virtual void DebugDraw() = 0;
	void InitializeObj();
	void UpdateColl();
};
