#include "Player.h"
#include "../Input/KeyboardInput.h"
#include "../Utility/Utility.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace NcmUtill;

std::shared_ptr<Model> Player::model_ = nullptr;
std::shared_ptr<Model> Player::sphere_model_ = nullptr;

Player::Player()
{
	object_ = std::make_shared<Object3d>();
	sphere_obj_ = std::make_shared<Object3d>();
}

Player::~Player()
{}

void Player::LoadResources()
{
	if (!model_)
	{
		model_ = std::make_shared<Model>();
		model_->LoadObjModel("Resources/low_fighter/", "new_low_fighter.obj", "new_low_fighter.mtl");

		sphere_model_ = std::make_shared<Model>();
		sphere_model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
	}
}

void Player::Initialize(MissileManager *mi_mgr, LockOnSystem *lockon_sys)
{
	mi_mgr_ = mi_mgr;
	lockon_sys_ = lockon_sys;

	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetPosition(XMFLOAT3(0, 0, 0));
	object_->SetRotation(XMFLOAT3(0, 180.0f, 0));
	object_->SetScale(1.0f);

	sphere_obj_->Initialize();
	sphere_obj_->SetModel(sphere_model_.get());

	UpdateCollision();
}

void Player::Finalize()
{}

void Player::Update()
{
	object_->Update();
	//lockon_sys_->Update();
	UpdateCollision();
}

void Player::Draw()
{
	object_->Draw();
	//lockon_sys_->Draw();
}

void Player::DrawColl()
{
	sphere_obj_->Draw();
}

void Player::DebugDraw()
{}

void Player::FireMissile(const MissileArgs &args)
{
	//mi_mgr_->Fire(args);

	MissileArgs l_args{};
	l_args.pos = object_->GetPosition();
	l_args.vel = XMFLOAT3(0, 0, 1.0f);
	l_args.acc = XMFLOAT3(0, 0, 0);
	// tgt_pos ‚ÍMissileManager‚ÅÝ’è
	// tgt_index ‚ÍMissileManager‚ÅÝ’è
	l_args.detection_range = 1000.0f;
	l_args.init_straight_time_ = 0;
	l_args.life = 100;
	l_args.is_alive = false;

	mi_mgr_->FireMultiMissile(l_args);
}

void Player::Move(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{
		XMFLOAT3 pos = object_->GetPosition();

		if (KeyboardInput::PushKey(DIK_W)) { pos.y += speed; }
		else if (KeyboardInput::PushKey(DIK_S)) { pos.y -= speed; }
		if (KeyboardInput::PushKey(DIK_D)) { pos.x += speed; }
		else if (KeyboardInput::PushKey(DIK_A)) { pos.x -= speed; }
		if (KeyboardInput::PushKey(DIK_R)) { pos.z += speed; }
		else if (KeyboardInput::PushKey(DIK_F)) { pos.z -= speed; }

		object_->SetPosition(pos);
	}

	if (KeyboardInput::PushKey(DIK_UP) || KeyboardInput::PushKey(DIK_DOWN) || KeyboardInput::PushKey(DIK_LEFT) || KeyboardInput::PushKey(DIK_RIGHT))
	{
		XMFLOAT3 rot = object_->GetRotation();

		if (KeyboardInput::PushKey(DIK_UP)) { rot.x -= speed; }
		if (KeyboardInput::PushKey(DIK_DOWN)) { rot.x += speed; }
		if (KeyboardInput::PushKey(DIK_LEFT)) { rot.z -= speed; }
		if (KeyboardInput::PushKey(DIK_RIGHT)) { rot.z += speed; }

		object_->SetRotation(rot);
	}
}

void Player::MoveXY(float speed)
{
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A))
	{
		XMFLOAT3 pos = object_->GetPosition();

		if (KeyboardInput::PushKey(DIK_W)) { pos.y += speed; }
		else if (KeyboardInput::PushKey(DIK_S)) { pos.y -= speed; }
		if (KeyboardInput::PushKey(DIK_D)) { pos.x += speed; }
		else if (KeyboardInput::PushKey(DIK_A)) { pos.x -= speed; }

		object_->SetPosition(pos);
	}
}

void Player::UpdateCollision()
{
	coll_.center = XMLoadFloat3(&object_->GetPosition());
	coll_.radius = COLL_RADIUS_;

	sphere_obj_->SetPosition(ToFloat3(coll_.center));
	sphere_obj_->SetScale(coll_.radius);
	sphere_obj_->Update();
}
