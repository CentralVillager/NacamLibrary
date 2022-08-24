#include "MainScene.h"
#include "../PreDraw/PreDraw.h"
#include "../Input/KeyboardInput.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../Collision/Collision.h"

MainScene::MainScene()
{
	// 各オブジェクトの生成
	camera_ = std::make_unique<Camera>();
	player_ = std::make_unique<Player>();
	enemy_ = std::make_unique<Enemy>();
	ene_list_ = std::make_unique<EnemiesList>();
	texture_ = std::make_unique<Sprite>();
	grid_ = std::make_unique<GridRender>();
	missile_mgr_ = std::make_unique<MissileManager>();
	dust_ = make_unique<Emitter>();
	sky_dome_ = make_unique<Object3d>();
	model_sky_dome_ = make_unique<Model>();
	reticle_ = make_unique<Reticle>();
	lockon_sys_ = make_unique<LockOnSystem>();

	// テクスチャ生成のテスト
	Sprite::GenerateTexture((int)(TexNum::Test), { 100, 100 }, 0xffff00ff);
	Sprite::LoadTexture((int)(TexNum::Test), L"Resources/Textures/Temp/orenge_texture.png");

	// 各リソースのロード
	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");
	Player::LoadResources();
	Enemy::LoadResources();
	Emitter::LoadResources();
	Missile::LoadResources();
	Reticle::LoadResources();
	LockOnSystem::LoadResources();
}

MainScene::~MainScene()
{}

void MainScene::Initialize()
{
	Object3d::SetCamera(camera_.get());
	GridRender::SetCamera(camera_.get());

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, 0));
	camera_->MoveEye(XMFLOAT3(0, 10.0f, 0));
	camera_->Update();

	sky_dome_->Initialize();
	sky_dome_->SetModel(model_sky_dome_.get());
	sky_dome_->SetScale(50.0f);
	sky_dome_->Update();

	lockon_sys_->Initialize(player_.get(), ene_list_.get());
	missile_mgr_->Initialize(lockon_sys_.get());
	player_->Initialize(missile_mgr_.get(), lockon_sys_.get());
	//enemy_->Initialize(XMFLOAT3(0, 0, 100.0f));
	grid_->Initialize(200, 10, XMFLOAT3(0, 0, 0));
	reticle_->Initialize();

	float offset = 10.0f;
	UINT ene_num = 10;
	for (UINT i = 0; i < ene_num; i++)
	{
		ene_list_->Add(XMFLOAT3(0 + offset * i, 0, 50));
		//ene_list_->Add(XMFLOAT3(0 + offset * i, 20, 50));
	}

	Emitter::EmitterArgs p;
	p.particle.position_ = { 0.0f, 0.0f, 0.0f };
	p.particle.velocity_ = { 0.0f, 0.0f, 0.0f };
	p.particle.accel_ = { 0, 0.001f, 0 };
	p.particle.life_ = 100;
	p.particle.s_scale_ = 0.1f;
	p.pos_rand_ = { 200.0f, 200.0f, 200.0f };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	dust_->SetEmitterArgs(p);

	texture_ = (std::unique_ptr<Sprite>)(Sprite::Create(1, { 0, 0 }));
	key_bind_ = (int)(KeyBind::Player);
}

void MainScene::Finalize()
{}

void MainScene::Update()
{
	// キーバインドごとの操作
	if (key_bind_ == (int)(KeyBind::Camera))
	{
		camera_->BasicCameraMoveTrack(2.0f);
	}
	else if (key_bind_ == (int)(KeyBind::Player))
	{
		// ミサイルの発射
		if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			MissileArgs args{};
			args.pos = player_->GetPos();
			args.vel = XMFLOAT3(0, 0, 1.0f);
			args.acc = XMFLOAT3(0, 0, 0);
			args.tgt_pos = lockon_sys_->GetTgtPos();
			args.detection_range = 1000.0f;
			args.init_straight_time_ = 0;
			args.life = 100;
			args.is_alive = false;
			player_->FireMissile(args);
		}

		player_->Move(1.0f);
	}
	else if (key_bind_ == (int)(KeyBind::AddEnemy))
	{
		if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			ene_list_->Add(XMFLOAT3(0, 0, 100));
		}
	}

	// リセット
	if (KeyboardInput::TriggerKey(DIK_2))
	{
		//camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, 0));
		camera_->SetEye(XMFLOAT3(0, 20.0f, -20.0f));
		camera_->SetTarget(XMFLOAT3(0, 10.0f, 0));
		player_->Initialize(missile_mgr_.get(), lockon_sys_.get());
	}

	// キーアサインの変更
	if (KeyboardInput::TriggerKey(DIK_RETURN))
	{
		key_bind_++;

		if (key_bind_ >= (int)(KeyBind::MaxNum))
		{
			key_bind_ = (int)(KeyBind::Start);
			key_bind_++;
		}
	}

	if (KeyboardInput::TriggerKey(DIK_0))
	{
		SceneManager::SetNextScene(SceneName::TITLE);
	}

	player_->Update();
	//enemy_->Update();
	ene_list_->Update();
	grid_->Update();
	missile_mgr_->Update();
	missile_mgr_->HomingTarget(*ene_list_);
	lockon_sys_->Update();

	for (UINT i = 0; i < ene_list_->GetEnemies().size(); i++)
	{
		if (missile_mgr_->CalcCollision(ene_list_->GetCollData(i)))
		{
			ene_list_->Kill(i);
		}
	}

	if (draw_dust_) { dust_->GenerateParticle(); }
	sky_dome_->Update();
}

void MainScene::Draw()
{
	PreDraw::PreRender(PipelineName::Line);
	grid_->Draw();

	PreDraw::PreRender(PipelineName::Object3d_WireFrame);
	player_->Draw();
	//enemy_->Draw();
	ene_list_->Draw();
	missile_mgr_->Draw();
	lockon_sys_->Draw();
	if (draw_dust_) { dust_->Draw(); }

	if (!is_wire_) { PreDraw::PreRender(PipelineName::Object3d); }
	sky_dome_->Draw();

	if (draw_coll_)
	{
		PreDraw::PreRender(PipelineName::Object3d_WireFrame);
		player_->DrawColl();
		//enemy_->DrawColl();
		ene_list_->DrawColl();
		missile_mgr_->DrawColl();
	}

	PreDraw::PreRender(PipelineName::Sprite);
	//texture_->Draw();
	reticle_->Draw();
}

void MainScene::DebugDraw()
{
	if (ImGui::CollapsingHeader("Draw"))
	{
		ImGui::Checkbox("WireFrame?", &is_wire_);
		ImGui::Checkbox("DrawDust?", &draw_dust_);
		ImGui::Checkbox("DrawCollision?", &draw_coll_);
		ImGui::Dummy(ImVec2(10.0f, 10.0f));
	}

	if (ImGui::RadioButton("Camera", &key_bind_, (int)(KeyBind::Camera)))
	{
		key_bind_ = (int)(KeyBind::Camera);
	}
	else if (ImGui::RadioButton("Player", &key_bind_, (int)(KeyBind::Player)))
	{
		key_bind_ = (int)(KeyBind::Player);
	}
	else if (ImGui::RadioButton("AddEnemy", &key_bind_, (int)(KeyBind::AddEnemy)))
	{
		key_bind_ = (int)(KeyBind::AddEnemy);
	}

	ImGui::Dummy(ImVec2(10.0f, 10.0f));
	missile_mgr_->DebugDraw();

	if (ImGui::CollapsingHeader("Missile"))
	{
		ImGui::DragFloat("Missile : DetectRange", &ImGui_detection_range_, 1.0f, 0.0f, 1000.0f);
	}
}
