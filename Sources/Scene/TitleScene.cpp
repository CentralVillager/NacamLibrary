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

	Sprite::LoadTexture((int)(TexNum::Title), L"Resources/Textures/SkyCircuse.png");
	Sprite::LoadTexture((int)(TexNum::Play_b), L"Resources/Textures/play_b.png");
	Sprite::LoadTexture((int)(TexNum::Play_w), L"Resources/Textures/play_w.png");
	Sprite::LoadTexture((int)(TexNum::Exit_b), L"Resources/Textures/exit_b.png");
	Sprite::LoadTexture((int)(TexNum::Exit_w), L"Resources/Textures/exit_w.png");
	Sprite::LoadTexture((int)(TexNum::Space), L"Resources/Textures/space.png");
	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");

	// タイトルテクスチャの設定
	title_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Title), { 0, 0 }));
	title_->SetSize({ 1280, 720 });
	play_b_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Play_b), { 100, 100 }));
	//play_b_->SetSize({ play_b_->GetTexSize().x / 2, play_b_->GetTexSize().y / 2 });
	play_w_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Play_w), { 0, 0 }));
	//play_w_->SetSize({ play_w_->GetTexSize().x / 2, play_w_->GetTexSize().y / 2 });
	exit_b_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Exit_b), { 0, 0 }));
	exit_w_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Exit_w), { 0, 0 }));
	space_ = (std::unique_ptr<Sprite>)(Sprite::Create((int)(TexNum::Space), { 0, 0 }));
	space_->SetAnchorPoint({ 0.5f, 0.5f });
	space_->SetSize({ space_->GetTexSize().x / 2, space_->GetTexSize().y / 2 });
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

	ImGui_pos_ = { 50, 500 };
	space_->SetPosition({ Win32App::window_center_x_, 650 });
	ImGui_pos2_ = space_->GetPosition();
}

void TitleScene::Finalize()
{}

void TitleScene::Update()
{
	if (KeyboardInput::TriggerKey(DIK_SPACE))
	{
		SceneManager::SetNextScene(SceneName::MAIN);
	}

	camera_->BasicCameraMoveTrack(1.0f);
	sky_dome_->Update();
	emitter_->GenerateParticle();

	space_->SetPosition(ImGui_pos2_);
}

void TitleScene::Draw()
{
	PreDraw::PreRender(PipelineName::Object3d);
	sky_dome_->Draw();
	emitter_->Draw();

	PreDraw::PreRender(PipelineName::Sprite);
	/*play_w_->Draw();
	play_b_->Draw();*/

	int visible_time = 75;
	int invisivle_time = 25;
	static int visi_count = visible_time;
	static int invi_count = invisivle_time;

	if (visi_count >= 0)
	{
		visi_count--;
		space_->Draw();
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

	title_->Draw();
}

void TitleScene::DebugDraw()
{
	ImGuiManager::DragFloat2("pos : play", ImGui_pos_, 1.0f, 0, 1280);
	ImGuiManager::DragFloat2("pos : space", ImGui_pos2_, 1.0f, 0, 1280);
}
