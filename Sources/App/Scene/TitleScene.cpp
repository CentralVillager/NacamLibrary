#include "TitleScene.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../Debug/ImGuiManager.h"
#include "../../Lib/Win32App/Win32App.h"
#include "../../Lib/PreDraw/PreDraw.h"
#include "../../Lib/Input/KeyboardInput.h"

TitleScene::TitleScene()
{
	camera_ = make_unique<Camera>();
	model_sky_dome_ = make_unique<Model>();
	sky_dome_ = make_unique<Object3d>();
	emitter_ = make_unique<Emitter>();

	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");

	title_ = Sprite::LoadTex(L"Resources/Textures/SkyCircuse.png");
	play_b_ = Sprite::LoadTex(L"Resources/Textures/play_b.png");
	play_w_ = Sprite::LoadTex(L"Resources/Textures/play_w.png");
	exit_b_ = Sprite::LoadTex(L"Resources/Textures/exit_b.png");
	exit_w_ = Sprite::LoadTex(L"Resources/Textures/exit_w.png");
	space_ = Sprite::LoadTex(L"Resources/Textures/space.png");

	Sprite::SetSize(title_, { 1280, 720 });

	XMFLOAT2 space_size = Sprite::GetSize(space_);
	Sprite::SetSize(space_, { space_size.x / 2, space_size.y / 2 });
	Sprite::SetAnchorPoint(space_, { 0.5f, 0.5f });
}

TitleScene::~TitleScene()
{}

void TitleScene::Initialize()
{
	camera_->Initialize();
	Object3d::SetCamera(camera_.get());

	sky_dome_->Initialize();
	sky_dome_->SetModel(model_sky_dome_.get());
	sky_dome_->SetScale(10.0f);
	sky_dome_->Update();

	Emitter::LoadResources();

	Emitter::EmitterArgs p;
	p.particle.position_ = { 0.0f, 0.0f, 500.0f };
	p.particle.velocity_ = { 0.0f, 0.0f, -10.0f };
	p.particle.accel_ = { 0.001f, 0.001f, 0 };
	p.particle.life_ = 100;
	p.particle.s_scale_ = 1.0f;
	p.pos_rand_ = { 500.0f, 500.0f, 0 };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	emitter_->SetEmitterArgs(p);

	XMINT2 pos = { Win32App::CENTER_.x, 650 };
	Sprite::SetPos(space_, pos);
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
		XMFLOAT2 pos = Sprite::GetPos(title_);
		pos.y += 1.0f;
		Sprite::SetPos(title_, pos);
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
		Sprite::DrawTex(space_);
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

	Sprite::DrawTex(title_);
}

void TitleScene::DebugDraw()
{}
