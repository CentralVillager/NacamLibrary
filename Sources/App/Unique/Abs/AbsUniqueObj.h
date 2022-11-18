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

	// ï`âÊÉfÅ[É^
	std::shared_ptr<Object3d> obj_;
	std::shared_ptr<Object3d> coll_obj_;

	// ìñÇΩÇËîªíË
	Sphere coll_;
	float coll_radius_;

	// ÇªÇÃëºèÓïÒ
	float speed_;
	bool is_dead_;

public:

	AbsUniqueObj(float speed, float coll_radius);
	virtual ~AbsUniqueObj() = default;

public:

	inline const XMFLOAT3 &GetPos() { return obj_->GetPos(); }
	inline const Sphere &GetCollData() { return coll_; }
	inline const bool &IsDead() { return is_dead_; }
	inline const void Death() { is_dead_ = true; }
	inline void SetPos(const XMFLOAT3 &pos) { obj_->SetPos(pos); }
	inline void SetIsDead(bool b) { is_dead_ = b; }

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawColl() = 0;
	virtual void DebugDraw() = 0;
	void InitObj3d(Model *obj_model, Model *coll_model);
	void UpdateColl();
};
