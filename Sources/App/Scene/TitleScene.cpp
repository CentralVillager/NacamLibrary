#include "TitleScene.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../DebugUtill/NcmImGui.h"
#include "../../Lib/Win32App/Win32App.h"
#include "../../Lib/PreDraw/PreDraw.h"
#include "../../Lib/Input/KeyboardInput.h"
#include "../../Lib/PostEffect/PostEffect.h"
#include "../../Lib/Input/NcmInput.h"

TitleScene::TitleScene()
{
	camera_ = make_unique<Camera>();
	model_sky_dome_ = make_unique<Model>();
	sky_dome_ = make_unique<Object3d>();
	emitter_ = make_unique<Emitter>();
	part_mgr_ = make_unique<NcmParticleManager>();

	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");

	title_ = NcmSprite::LoadTex("Resources/Textures/SkyCircuse.png");
	play_b_ = NcmSprite::LoadTex("Resources/Textures/play_b.png");
	play_w_ = NcmSprite::LoadTex("Resources/Textures/play_w.png");
	exit_b_ = NcmSprite::LoadTex("Resources/Textures/exit_b.png");
	exit_w_ = NcmSprite::LoadTex("Resources/Textures/exit_w.png");
	space_ = NcmSprite::LoadTex("Resources/Textures/space.png");

	NcmSprite::SetSize(title_, { 1280, 720 });

	XMFLOAT2 space_size = NcmSprite::GetSize(space_);
	NcmSprite::SetSize(space_, { space_size.x / 2, space_size.y / 2 });
	NcmSprite::SetAnchorPoint(space_, { 0.5f, 0.5f });
}

TitleScene::~TitleScene()
{
	NcmSprite::TermSprite();
}

void TitleScene::Initialize()
{
	// カメラの初期化
	camera_->Initialize();
	Object3d::SetCamera(camera_.get());

	// 天球の初期化
	sky_dome_->Initialize();
	sky_dome_->SetModel(model_sky_dome_.get());
	sky_dome_->SetScale(10.0f);
	sky_dome_->Update();

	// エミッターのリソースをロード
	Emitter::LoadResources();

	// エミッターの設定
	EmitterDesc p;
	p.part_desc_.position_ = { 0.0f, 0.0f, 500.0f };
	p.part_desc_.velocity_ = { 0.0f, 0.0f, -10.0f };
	p.part_desc_.accel_ = { 0.001f, 0.001f, 0 };
	p.part_desc_.life_ = 100;
	p.part_desc_.s_scale_ = 50.0f;
	p.part_desc_.e_scale_ = 0;
	p.pos_rand_ = { 500.0f, 500.0f, 0 };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	emitter_->SetEmitterDesc(p);

	// スプライトの設定
	XMINT2 pos = { Win32App::CENTER_.x, 650 };
	NcmSprite::SetPos(space_, pos);

	is_scene_change_ = false;

	part_mgr_->Initialize();
	NcmPlatePoly::SetCamera(camera_.get());
}

void TitleScene::Finalize()
{}

void TitleScene::Update()
{
	NcmParticleManager::ClearParticleArgsBeforeUpdate();

	if (!is_scene_change_)
	{
		SceneManager::OutChangeScene(0.05f);
	}

	// シーン遷移
	if (KeyboardInput::TriggerKey(DIK_SPACE) ||
		NcmInput::IsTrigger(NcmButtonType::A) ||
		NcmInput::IsTrigger(NcmButtonType::B) ||
		NcmInput::IsTrigger(NcmButtonType::X) ||
		NcmInput::IsTrigger(NcmButtonType::Y))
	{
		is_scene_change_ = true;
	}

	if (is_scene_change_)
	{
		if (SceneManager::InChangeScene(0.05f))
		{
			SceneManager::SetNextScene(SceneName::MAIN);
		}
	}

	camera_->BasicCameraMoveTrack(1.0f);
	sky_dome_->Update();
	emitter_->GenerateParticle();
	emitter_->UpdateParticle();
	part_mgr_->Update();
}

void TitleScene::Draw()
{
	PreDraw::SetPipeline(PipelineName::Object3d);
	sky_dome_->Draw();

	PreDraw::SetPipeline(PipelineName::PlatePoly);
	part_mgr_->Draw();

	PreDraw::SetPipeline(PipelineName::Sprite);

	int visible_time = 75;
	int invisivle_time = 25;
	static int visi_count = visible_time;
	static int invi_count = invisivle_time;

	if (visi_count >= 0)
	{
		visi_count--;
		NcmSprite::DrawTex(space_);
	}
	else
	{
		invi_count--;

		if (invi_count <= 0)
		{
			visi_count = visible_time;
			invi_count = invisivle_time;
		}
	}

	NcmSprite::DrawTex(title_);
}

void TitleScene::DebugDraw()
{
	NcmParticleManager::StaticDebugDraw();
}
