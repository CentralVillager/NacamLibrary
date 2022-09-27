#include "TitleScene.h"
#include "../Win32App/Win32App.h"
#include "../PreDraw/PreDraw.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../Input/KeyboardInput.h"
#include "../Debug/ImGuiManager.h"

TitleScene::TitleScene()
{
	camera_ = make_unique<Camera>();
	model_sky_dome_ = make_unique<Model>();
	sky_dome_ = make_unique<Object3d>();
	emitter_ = make_unique<Emitter>();

	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");

	title_ = SpriteManager::LoadTex(L"Resources/Textures/SkyCircuse.png");
	play_b_ = SpriteManager::LoadTex(L"Resources/Textures/play_b.png");
	play_w_ = SpriteManager::LoadTex(L"Resources/Textures/play_w.png");
	exit_b_ = SpriteManager::LoadTex(L"Resources/Textures/exit_b.png");
	exit_w_ = SpriteManager::LoadTex(L"Resources/Textures/exit_w.png");
	space_ = SpriteManager::LoadTex(L"Resources/Textures/space.png");

	SpriteManager::SetSize(title_, { 1280, 720 });

	XMFLOAT2 space_size = SpriteManager::GetSize(space_);
	SpriteManager::SetSize(space_, { space_size.x / 2, space_size.y / 2 });
	SpriteManager::SetAnchorPoint(space_, { 0.5f, 0.5f });
}

TitleScene::~TitleScene()
{}

void TitleScene::Initialize()
{
	camera_->Initialize();
	Object3d::SetCamera(camera_.get());

	sky_dome_->Initialize();
	sky_dome_->SetModel(model_sky_dome_.get());
	sky_dome_->SetScale(2.0f);
	sky_dome_->Update();

	Emitter::LoadResources();

	Emitter::EmitterArgs p;
	p.particle.position_ = { 0.0f, 0.0f, 200.0f };
	p.particle.velocity_ = { 0.0f, 0.0f, -4.0f };
	p.particle.accel_ = { 0.001f, 0.001f, 0 };
	p.particle.life_ = 100;
	p.particle.s_scale_ = 1.0f;
	p.pos_rand_ = { 200.0f, 200.0f, 0 };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	emitter_->SetEmitterArgs(p);

	SpriteManager::SetPos(space_, { Win32App::window_center_x_, 650 });
}

void TitleScene::Finalize()
{}

void TitleScene::Update()
{
	if (KeyboardInput::TriggerKey(DIK_SPACE))
	{
		SceneManager::SetNextScene(SceneName::MAIN);
	}

	if (KeyboardInput::PushKey(DIK_DOWN))
	{
		XMFLOAT2 pos = SpriteManager::GetPos(title_);
		pos.y += 1.0f;
		SpriteManager::SetPos(title_, pos);
	}

	camera_->BasicCameraMoveTrack(1.0f);
	sky_dome_->Update();
	emitter_->GenerateParticle();
}

void TitleScene::Draw()
{
	PreDraw::PreRender(PipelineName::Object3d);
	sky_dome_->Draw();
	emitter_->Draw();

	PreDraw::PreRender(PipelineName::Sprite);

	int visible_time = 75;
	int invisivle_time = 25;
	static int visi_count = visible_time;
	static int invi_count = invisivle_time;

	if (visi_count >= 0)
	{
		visi_count--;
		SpriteManager::DrawTex(space_);
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

	SpriteManager::DrawTex(title_);
}

void TitleScene::DebugDraw()
{}
