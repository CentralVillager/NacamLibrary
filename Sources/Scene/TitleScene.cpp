#include "TitleScene.h"
#include "../Win32App/Win32App.h"
#include "../PreDraw/PreDraw.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../Input/KeyboardInput.h"

TitleScene::TitleScene()
{
	camera_ = make_unique<Camera>();
	model_sky_dome_ = make_unique<Model>();
	sky_dome_ = make_unique<Object3d>();
	emitter_ = make_unique<Emitter>();

	Sprite::LoadTexture(0, L"Resources/Textures/SkyCircuse.png");
	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");
}

TitleScene::~TitleScene()
{}

void TitleScene::Initialize()
{
	camera_->Initialize();
	Object3d::SetCamera(camera_.get());

	// タイトルテクスチャの設定
	title_ = (std::unique_ptr<Sprite>)(Sprite::Create(0, { 0, 0 }));
	title_->SetSize({ 1280, 720 });

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
}

void TitleScene::Finalize()
{}

void TitleScene::Update()
{
	if (KeyboardInput::TriggerKey(DIK_0))
	{
		SceneManager::SetNextScene(SceneName::MAIN);
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
	title_->Draw();
}

void TitleScene::DebugDraw()
{}
