#include "DemoScene.h"
#include "../../../Lib/3D/FbxLoader.h"
#include "../../../Lib/2D/NcmSprite.h"
#include "../../../Lib/Win32App/Win32App.h"
#include "../../../Lib/DirectX/PreDraw.h"
#include "../../../Lib/Input/KeyboardInput.h"

using namespace std;
using namespace DirectX;

DemoScene::DemoScene()
{
	// カメラの生成
	camera_ = make_unique<Camera>();

	texture_ = NcmSprite::LoadTex("Resources/Textures/orange_circle.png");

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

	tex_obj_ = make_unique<Object3d>();
	tex_model_ = make_unique<Model>();
	tex_model_->LoadObjModel("Resources/ball/", "smooth_ball.obj", "tex_ball.mtl");

	dds_ = NcmSprite::LoadTex("Resources/Ball/background.dds");
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

	PlatePoly::SetCamera(camera_.get());
	poly_->Initialize(texture_);

	GridRender::SetCamera(camera_.get());
	grid_->Initialize(200, 10, XMFLOAT3(0, 0, 0));

	Object3d::SetCamera(camera_.get());
	player_->Initialize();

	tex_obj_->SetModel(tex_model_.get());
	tex_obj_->Initialize();
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

	camera_->BasicCameraMoveTrack(1.0f);
	camera_->Update();
	poly_->Update();
	grid_->Update(0.0f);

	for (auto &i : objs_)
	{
		i.Update();
	}

	tex_obj_->Update();
}

void DemoScene::Draw()
{
	using enum PipelineName;

	PreDraw::SetPipeline(Object3d_WireFrame);

	for (auto &i : objs_)
	{
		//i.Draw();
	}

	PreDraw::SetPipeline(Object3d);
	tex_obj_->Draw();

	PreDraw::SetPipeline(Line);
	grid_->Draw();

	PreDraw::SetPipeline(Sprite);
	NcmSprite::DrawTex(dds_);

	PreDraw::SetPipeline(PlatePoly);
	poly_->Draw();
}

void DemoScene::AfterPostEffectDraw()
{}

void DemoScene::DebugDraw()
{
	camera_->DebugDraw();
}
