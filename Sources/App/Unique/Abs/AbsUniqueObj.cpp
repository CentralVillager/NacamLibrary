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
	// �����蔻��̒��S��`��f�[�^�̈ʒu��
	coll_.center = XMLoadFloat3(&obj_->GetPos());

	// �����蔻��ʒu��XMFLOAT3�֕ϊ�
	XMFLOAT3 dist;
	XMStoreFloat3(&dist, coll_.center);

	// �����X�V
	coll_obj_->SetPos(dist);
	coll_obj_->SetScale(coll_.radius);
	coll_obj_->Update();
}

void AbsUniqueObj::CalcFwdVec()
{
	// ���ʃx�N�g����������
	forward_vec_ = XMFLOAT3(0, 0, -1);

	// XMVECTOR�֕ϊ�
	XMVECTOR forward_vec = XMLoadFloat3(&forward_vec_);

	// ��]�s��𐶐�
	mat_rot_ = XMMatrixIdentity();
	mat_rot_ *= XMMatrixRotationZ(XMConvertToRadians(obj_->GetRot().z));
	mat_rot_ *= XMMatrixRotationX(XMConvertToRadians(obj_->GetRot().x));
	mat_rot_ *= XMMatrixRotationY(XMConvertToRadians(obj_->GetRot().y));

	// ���ʃx�N�g����
	forward_vec = XMVector3TransformCoord(forward_vec, mat_rot_);

	// ���K��
	forward_vec = XMVector3Normalize(forward_vec);

	// ���ʃx�N�g���֑��
	XMStoreFloat3(&forward_vec_, forward_vec);
}
