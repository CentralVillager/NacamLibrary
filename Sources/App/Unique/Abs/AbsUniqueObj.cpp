#include "AbsUniqueObj.h"
#include "../../Utility/NcmUtil.h"

using namespace DirectX;

AbsUniqueObj::AbsUniqueObj(float speed, float coll_radius)
	:
	mat_rot_(XMMatrixIdentity()),
	obj_(std::make_shared<Object3d>()),
	coll_obj_(std::make_shared<Object3d>()),
	coll_(),
	forward_vec_(0, 0, -1),
	speed_(speed),
	is_dead_(true)
{
	coll_.radius = coll_radius;
}

void AbsUniqueObj::InitObj3d(Model *obj_model, Model *coll_model)
{
	obj_->Initialize();
	obj_->SetModel(obj_model);
	coll_obj_->Initialize();
	coll_obj_->SetModel(coll_model);
}

void AbsUniqueObj::UpdateColl()
{
	// 当たり判定の中心を描画データの位置に
	coll_.center = XMLoadFloat3(&obj_->GetPos());

	// 当たり判定位置をXMFLOAT3へ変換
	XMFLOAT3 dist;
	XMStoreFloat3(&dist, coll_.center);

	// 情報を更新
	coll_obj_->SetPos(dist);
	coll_obj_->SetScale(coll_.radius);
	coll_obj_->Update();
}

void AbsUniqueObj::CalcFwdVec()
{
	// 正面ベクトルを初期化
	forward_vec_ = XMFLOAT3(0, 0, -1);

	// XMVECTORへ変換
	XMVECTOR forward_vec = XMLoadFloat3(&forward_vec_);

	// 回転行列を生成
	mat_rot_ = XMMatrixIdentity();
	mat_rot_ *= XMMatrixRotationZ(XMConvertToRadians(obj_->GetRot().z));
	mat_rot_ *= XMMatrixRotationX(XMConvertToRadians(obj_->GetRot().x));
	mat_rot_ *= XMMatrixRotationY(XMConvertToRadians(obj_->GetRot().y));

	// 正面ベクトルへ
	forward_vec = XMVector3TransformCoord(forward_vec, mat_rot_);

	// 正規化
	forward_vec = XMVector3Normalize(forward_vec);

	// 正面ベクトルへ代入
	XMStoreFloat3(&forward_vec_, forward_vec);
}
