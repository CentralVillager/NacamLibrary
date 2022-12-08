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

	texture_ = NcmSprite::LoadTex(L"Resources/Textures/effect1.png");
	test_ = NcmSprite::LoadTex(L"Resources/Textures/Temp/orenge_texture.png");

	point_ = make_unique<Point>();
	poly_ = make_unique<PlatePoly>();

	grid_ = make_unique<GridRender>();
	ugrid_ = make_unique<GridRender>();

	player_ = make_unique<Player>();

	Player::LoadResources();

	// レベルデータの読み込み
	level_ = make_unique<LevelDataManager>();
	level_->LoadLevelByJson("Resources/Levels/sample.json");

	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/ball/", "smooth_ball.obj", "smooth_ball.mtl");

	particle_ = make_unique<NcmPlatePoly>();
}

DemoScene::~DemoScene()
{}

void DemoScene::Initialize()
{
	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack({ 0, 5.0f, 0 });

	// それぞれのデータを適用
	for (uint32_t i = 0; i < level_->GetLevelDatas()->object_data.size(); i++)
	{
		objs_.emplace_back();
		Object3d &obj_ptr = objs_.back();
		obj_ptr.Initialize();
		obj_ptr.SetModel(model_.get());
		obj_ptr.SetPos(level_->GetLevelDatas()->object_data[i].pos);
		obj_ptr.SetRot(level_->GetLevelDatas()->object_data[i].rot);
		obj_ptr.SetScale(level_->GetLevelDatas()->object_data[i].scale.x);
	}

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
	grid_->Initialize(200, 10, XMFLOAT3(0, 0, 0));
	//grid_->Initialize(200, 10, XMFLOAT3(0, -20.0f, 0));
	//ugrid_->Initialize(200, 10, XMFLOAT3(0, +50.0f, 0));

	Object3d::SetCamera(camera_.get());
	player_->Initialize();

	NcmPlatePoly::SetCamera(camera_.get());
	particle_->Initialize();
}

void DemoScene::Finalize()
{}

void DemoScene::Update()
{
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

	//player_->RotationY(1.0f);
	//player_->MoveXZ(1.0f);

	//camera_->FollowCameraMove(1.0f, *player_);
	camera_->BasicCameraMoveTrack(1.0f);
	camera_->Update();
	poly_->Update();
	grid_->Update();
	//ugrid_->Update();
	//player_->Update();

	for (auto &i : objs_)
	{
		i.Update();
	}

	//particle_->Update();
}

void DemoScene::Draw()
{
	using enum PipelineName;

	PreDraw::SetPipeline(Object3d_WireFrame);
	//player_->Draw();

	for (auto &i : objs_)
	{
		i.Draw();
	}

	PreDraw::SetPipeline(Line);
	grid_->Draw();
	//ugrid_->Draw();

	PreDraw::SetPipeline(Sprite);
	/*NcmSprite::DrawTex(texture_);
	NcmSprite::DrawTex(test_, Win32App::FCENTER_);*/

	PreDraw::SetPipeline(PlatePoly);
	poly_->Draw();

	particle_->Draw();
}

void DemoScene::DebugDraw()
{
	camera_->DebugDraw();
}
;