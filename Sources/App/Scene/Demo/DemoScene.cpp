#include "DemoScene.h"
#include "../../../Lib/Fbx/FbxLoader.h"
#include "../Sources/Lib/Sprite/NcmSprite.h"
#include "../Sources/Lib/Win32App/Win32App.h"
#include "../Sources/Lib/PreDraw/PreDraw.h"
#include "../Sources/Lib/Input/KeyboardInput.h"

using namespace std;
using namespace DirectX;

DemoScene::DemoScene()
{
	// カメラの生成
	camera_ = make_unique<Camera>();

	// FBXモデルデータの生成
	/*fbx_model_ = make_unique<FbxModel>();

	fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");*/
	//fbx_model_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//fbx_obj_ = new FbxObject3d();

	texture_ = NcmSprite::LoadTex(L"Resources/Textures/effect1.png");
	test_ = NcmSprite::LoadTex(L"Resources/Textures/Temp/orenge_texture.png");

	point_ = make_unique<Point>();
	poly_ = make_unique<PlatePoly>();

	grid_ = make_unique<GridRender>();
	ugrid_ = make_unique<GridRender>();

	player_ = make_unique<Player>();

	Player::LoadResources();
}

DemoScene::~DemoScene()
{}

void DemoScene::Initialize()
{
	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });
	//FbxObject3d::SetCamera(camera_.get());

	//// FBXオブジェクトデータの初期化
	//fbx_obj_->Initialize();
	//fbx_obj_->SetModel(fbx_model_);
	//fbx_obj_->SetScale({ 1.0f, 1.0f, 1.0f });
	//fbx_obj_->SetPosition({ 0, 0, 0 });

	NcmEaseDesc ease;
	ease.init_value = 0.0f;
	ease.total_move = (float)(Win32App::SIZE_.y);
	ease.ease_type = NcmEaseType::OutCubic;
	ease_ = NcmEasing::RegisterEaseData(ease);

	NcmSprite::SetAnchorPoint(texture_, { 0.0f, 0.5f });
	NcmSprite::SetPos(texture_, XMFLOAT2(0.0f, (float)(Win32App::CENTER_.y)));

	NcmSprite::SetAnchorPoint(test_, { 0.5f, 0.5f });
	NcmSprite::SetPos(test_, Win32App::FCENTER_);

	PlatePoly::SetCamera(camera_.get());
	poly_->Initialize(texture_);
	//poly_->SetTexSRVs(texture_);

	GridRender::SetCamera(camera_.get());
	grid_->Initialize(200, 10, XMFLOAT3(0, -20.0f, 0));
	ugrid_->Initialize(200, 10, XMFLOAT3(0, +50.0f, 0));

	Object3d::SetCamera(camera_.get());
	player_->Initialize();
}

void DemoScene::Finalize()
{}

void DemoScene::Update()
{
	// 回転処理
	//float rotation_y = fbx_obj_->GetRotation().y;
	//rotation_y += 1.0f;
	//fbx_obj_->SetRotation({ 0, rotation_y, 0 });

	//// 更新
	//fbx_obj_->PlayAnimation(0, true);
	//fbx_obj_->Update();

	if (KeyboardInput::TriggerKey(DIK_SPACE))
	{
		NcmEasing::ResetTime(ease_);
		b = !b;
	}

	if (b)
	{
		NcmEasing::SetInitValue(ease_, (float)(Win32App::SIZE_.x));
		NcmEasing::SetTotalMove(ease_, -1280.0f);
	}
	else
	{
		NcmEasing::SetInitValue(ease_, 0);
		NcmEasing::SetTotalMove(ease_, 1280.0f);
	}

	NcmEasing::UpdateValue(ease_);

	NcmSprite::SetPos(texture_, XMFLOAT2(NcmEasing::GetValue(ease_), (float)(Win32App::CENTER_.y)));

	float speed = 0.5f;

	/*if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{
		XMFLOAT3 pos = poly_->GetPos();

		if (KeyboardInput::PushKey(DIK_W)) { pos.y += speed; }
		else if (KeyboardInput::PushKey(DIK_S)) { pos.y -= speed; }
		if (KeyboardInput::PushKey(DIK_D)) { pos.x += speed; }
		else if (KeyboardInput::PushKey(DIK_A)) { pos.x -= speed; }
		if (KeyboardInput::PushKey(DIK_R)) { pos.z += speed; }
		else if (KeyboardInput::PushKey(DIK_F)) { pos.z -= speed; }

		poly_->SetPos(pos);
	}*/

	//player_->RotationY(1.0f);
	//player_->MoveXZ(1.0f);

	camera_->TestCameraMove(1.0f, *player_);
	camera_->Update();
	poly_->Update();
	grid_->Update();
	ugrid_->Update();
	player_->Update();

	XMFLOAT3 target = player_->GetPos();
	target.z += 50.0f;
	camera_->SetTarget(target);
	XMFLOAT3 eye = player_->GetPos();
	eye.y += 20.0f;
	eye.z -= 30.0f;
	camera_->SetEye(eye);
}

void DemoScene::Draw()
{
	//fbx_obj_->Draw(cmd_list_.Get());

	using enum PipelineName;

	PreDraw::PreRender(Object3d_WireFrame);
	player_->Draw();

	PreDraw::PreRender(Line);
	grid_->Draw();
	ugrid_->Draw();

	PreDraw::PreRender(Sprite);
	/*NcmSprite::DrawTex(texture_);
	NcmSprite::DrawTex(test_, Win32App::FCENTER_);*/

	PreDraw::PreRender(PlatePoly);
	poly_->Draw();
}

void DemoScene::DebugDraw()
{
	camera_->DebugDraw();
}
;