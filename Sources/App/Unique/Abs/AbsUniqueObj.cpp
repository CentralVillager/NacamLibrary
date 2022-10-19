#include "AbsUniqueObj.h"
#include "../../Utility/NcmUtil.h"

AbsUniqueObj::AbsUniqueObj(float speed, float coll_radius)
	:
	obj_(std::make_shared<Object3d>()),
	coll_obj_(std::make_shared<Object3d>()),
	coll_radius_(coll_radius),
	speed_(speed),
	is_dead_(true)
{}

void AbsUniqueObj::InitObj3d(Model *obj_model, Model *coll_model)
{
	obj_->Initialize();
	obj_->SetModel(obj_model);
	coll_obj_->Initialize();
	coll_obj_->SetModel(coll_model);
}

void AbsUniqueObj::UpdateColl()
{
	coll_.center = DirectX::XMLoadFloat3(&obj_->GetPosition());
	coll_.radius = coll_radius_;

	XMFLOAT3 dist;
	DirectX::XMStoreFloat3(&dist, coll_.center);
	coll_obj_->SetPosition(dist);
	coll_obj_->SetScale(coll_.radius);
	coll_obj_->Update();
}
