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
	grid_ = std::make_unique<GridRender>();
	missile_mgr_ = std::make_unique<MissileManager>();
	dust_ = make_unique<Emitter>();
	sky_dome_ = make_unique<Object3d>();
	model_sky_dome_ = make_unique<Model>();
	reticle_ = make_unique<Reticle>();
	lockon_sys_ = make_unique<LockOnSystem>();

	// テクスチャのロード
	clear_ = Sprite::LoadTex(L"Resources/Textures/clear.png");
	space_ = Sprite::LoadTex(L"Resources/Textures/space.png");

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

	lockon_sys_->Initialize(player_.get(), ene_list_.get(), 4);
	missile_mgr_->Initialize(lockon_sys_.get());
	player_->Initialize(missile_mgr_.get(), lockon_sys_.get());
	grid_->Initialize(200, 10, XMFLOAT3(0, 0, 0));
	reticle_->Initialize();

	// enemyの生成
	float offset = 10.0f;
	float z_offset = 50.0f;
	UINT ene_num = 10;

	ene_list_->Add(XMFLOAT3(0, 0, z_offset));
	for (UINT i = 0; i < ene_num / 2; i++)
	{
		ene_list_->Add(XMFLOAT3(offset * i + offset, 0, z_offset * i + z_offset));
	}
	for (UINT i = 0; i < ene_num / 2; i++)
	{
		ene_list_->Add(XMFLOAT3(-(offset * i + offset), 0, z_offset * i + z_offset));
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

	Sprite::SetSize(clear_, { 1280, 720 });

	XMINT2 pos = { Win32App::CENTER_.x, 650 };
	XMFLOAT2 size = Sprite::GetSize(space_);
	Sprite::SetPos(space_, pos);
	Sprite::SetSize(space_, { size.x / 2, size.y / 2 });
	Sprite::SetAnchorPoint(space_, { 0.5f, 0.5f });

	// キーバインド機能を使用するか
#ifdef _DEBUG
	use_keybind_ = true;
	key_bind_ = (int)(KeyBind::Player);
#else
	use_keybind_ = false;
#endif

	is_result_ = false;
}

void MainScene::Finalize()
{}

void MainScene::Update()
{
	if (use_keybind_)
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
				player_->FireMissile();
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

		// 強制シーン遷移
		if (KeyboardInput::TriggerKey(DIK_0))
		{
			SceneManager::SetNextScene(SceneName::RESULT);
		}
	}
	else if (!use_keybind_)
	{
		// ミサイル発射
		if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			player_->FireMissile();
		}
	}

	// 強制リザルト
	/*if (KeyboardInput::TriggerKey(DIK_0))
	{
		is_result_ = true;
	}*/

	// クリア遷移
	/*if (ene_list_->NoticeEmpty())
	{
		is_result_ = true;

		if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			SceneManager::SetNextScene(SceneName::TITLE);
		}
	}*/

	player_->MoveXY(1.0f);

	// 各オブジェクト更新処理
	player_->Update();
	ene_list_->Update();
	grid_->Update();
	missile_mgr_->Update();
	lockon_sys_->Update();

	// ミサイル追尾処理
	missile_mgr_->HomingTarget(*ene_list_);

	// 当たり判定
	for (UINT i = 0; i < ene_list_->GetEnemies().size(); i++)
	{
		if (missile_mgr_->CalcCollision(ene_list_->GetCollData(i)))
		{
			// 死亡処理
			ene_list_->Death(i);
		}
	}

	// 塵描画他
	if (draw_dust_) { dust_->GenerateParticle(); }
	sky_dome_->Update();
}

void MainScene::Draw()
{
	PreDraw::PreRender(PipelineName::Line);
	grid_->Draw();

	PreDraw::PreRender(PipelineName::Object3d_WireFrame);
	player_->Draw();
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
		ene_list_->DrawColl();
		missile_mgr_->DrawColl();
	}

	PreDraw::PreRender(PipelineName::Sprite);
	if (is_result_)
	{
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

		Sprite::DrawTex(clear_);
	}
}

void MainScene::DebugDraw()
{
	if (ImGui::CollapsingHeader("Draw"))
	{
		ImGui::Checkbox("DrawWireFrame?", &is_wire_);
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
