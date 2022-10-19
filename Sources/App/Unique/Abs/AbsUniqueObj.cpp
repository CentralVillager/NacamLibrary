#include "AbsUniqueObj.h"
#include "../../Utility/NcmUtil.h"

std::unique_ptr<Model> AbsUniqueObj::model_ = nullptr;
std::unique_ptr<Model> AbsUniqueObj::coll_model_ = nullptr;

AbsUniqueObj::AbsUniqueObj(XMFLOAT3 pos, float speed)
	:
	obj_(std::make_unique<Object3d>()),
	coll_obj_(std::make_unique<Object3d>()),
	COLL_RADIUS_(1.0f),
	pos_(pos),
	speed_(speed),
	is_dead_(true)
{}

void AbsUniqueObj::InitializeObj()
{
	obj_->Initialize();
	obj_->SetModel(model_.get());

	coll_obj_->Initialize();
	coll_obj_->SetModel(coll_model_.get());
}

void AbsUniqueObj::UpdateColl()
{
	coll_.center = DirectX::XMLoadFloat3(&obj_->GetPosition());
	coll_.radius = COLL_RADIUS_;

	coll_obj_->SetPosition(NcmUtill::ToFloat3(coll_.center));
	coll_obj_->SetScale(coll_.radius);
	coll_obj_->Update();
}
