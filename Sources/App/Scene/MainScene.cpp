#include "MainScene.h"
#include "../../Lib/PreDraw/PreDraw.h"
#include "../../Lib/Input/KeyboardInput.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../Sources/App/Collision/Collision.h"
#include "../Sources/App/Math/Easing/NcmEasing.h"
#include "../Utility/NcmUtil.h"

using namespace NcmUtill;

MainScene::MainScene() :
	camera_(std::make_unique<Camera>()),
	player_(std::make_unique<Player>()),
	enemy_(std::make_unique<Enemy>()),
	ene_list_(std::make_unique<EnemiesList>()),
	grid_(std::make_unique<GridRender>()),
	missile_mgr_(std::make_unique<MissileManager>()),
	dust_(make_unique<Emitter>()),
	sky_dome_(make_unique<Object3d>()),
	model_sky_dome_(make_unique<Model>()),
	reticle_(make_unique<Reticle>()),
	lockon_sys_(make_unique<LockOnSystem>()),
	numbers_(make_unique<Numbers>()),
	ui_(make_unique<NcmUi>()),
	texture_(),
	clear_(),
	over_(),
	space_(),
	do_debug_(true),
	key_bind_(0),
	is_wire_(true),
	draw_dust_(true),
	draw_coll_(false),
	draw_numbers_(false),
	is_clear_(false),
	is_failed_(false),
	ImGui_detection_range_(1000.0f),
	ImGui_Ui_pos_(Win32App::FCENTER_),
	player_camera_speed_()
{
	// テクスチャのロード
	clear_ = NcmSprite::LoadTex(L"Resources/Textures/clear.png");
	over_ = NcmSprite::LoadTex(L"Resources/Textures/SC/over.png");
	space_ = NcmSprite::LoadTex(L"Resources/Textures/space.png");

	// 各リソースのロード
	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");
	Player::LoadResources();
	Enemy::LoadResources();
	Emitter::LoadResources();
	Missile::LoadResources();
	Reticle::LoadResources();
	LockOnSystem::LoadResources();
	NcmUi::LoadResources();
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
	camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, cam_init_pos_.z));
	//camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, -10.0f));
	//camera_->MoveEye(cam_init_pos_);
	camera_->MoveEye(XMFLOAT3(0, 10.0f, -10.0f));
	camera_->Update();

	// 天球の初期化
	sky_dome_->Initialize();
	sky_dome_->SetModel(model_sky_dome_.get());
	sky_dome_->SetScale(50.0f);
	sky_dome_->Update();

	// 各ゲームオブジェクトの初期化
	ene_list_->Initialize(player_.get());
	lockon_sys_->Initialize(player_.get(), ene_list_.get());
	missile_mgr_->Initialize(lockon_sys_.get());
	player_->Initialize(missile_mgr_.get(), lockon_sys_.get(), init_pos_);
	grid_->Initialize(200, 10, XMFLOAT3(0, -20.0f, 0));
	reticle_->Initialize();
	numbers_->Initialize();
	/*for (UINT i = 0; i < grid_floor_.size(); i++)
	{
		grid_floor_[i].Initialize(200, 10, XMFLOAT3(0, 0, (float)(i) * 1000));
	}*/

	// enemyの生成
	ene_list_->AddTemplateSet();

	// 塵エミッターの生成
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

	// スプライトの初期化
	NcmSprite::SetSize(clear_, { 1280.0f, 720.0f });
	NcmSprite::SetSize(over_, { 1280.0f, 720.0f });

	XMINT2 pos = { Win32App::CENTER_.x, 650 };
	XMFLOAT2 size = NcmSprite::GetSize(space_);
	NcmSprite::SetPos(space_, pos);
	NcmSprite::SetSize(space_, { size.x / 2, size.y / 2 });
	NcmSprite::SetAnchorPoint(space_, { 0.5f, 0.5f });

	// キーバインド機能を使用するか
#ifdef _DEBUG
	do_debug_ = true;
	key_bind_ = (int)(KeyBind::Player);
#else
	do_debug_ = false;
#endif

	is_clear_ = false;

	EaseArgs ease;
	ease.ease_type = EaseType::OutCirc;
	ease.init_value = 0.0f;
	ease.total_move = SPEED_;
	player_camera_speed_ = NcmEasing::RegisterEaseData(ease);
}

void MainScene::Finalize()
{}

void MainScene::Update()
{
	if (do_debug_)
	{
		// キーバインドごとの操作
		if (key_bind_ == (int)(KeyBind::Camera))
		{
			camera_->BasicCameraMoveTrack(2.0f);
		}
		else if (key_bind_ == (int)(KeyBind::Player))
		{
			if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A))
			{
				NcmEasing::UpdateValue(player_camera_speed_);
			}
			else
			{
				NcmEasing::ResetTime(player_camera_speed_);
			}

			player_->MoveXZ(NcmEasing::GetValue(player_camera_speed_));
			camera_->MoveXY(NcmEasing::GetValue(player_camera_speed_));
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
			camera_->SetEye(XMFLOAT3(0, 10.0f, -10.0f));
			//camera_->SetTarget(cam_init_pos_);
			player_->Initialize(missile_mgr_.get(), lockon_sys_.get(), init_pos_);
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
			is_clear_ = true;
			SceneManager::SetNextScene(SceneName::RESULT);
		}
	}
	else if (!do_debug_)
	{
		if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A))
		{
			NcmEasing::UpdateValue(player_camera_speed_);
		}
		else
		{
			NcmEasing::ResetTime(player_camera_speed_);
		}

		player_->MoveXZ(NcmEasing::GetValue(player_camera_speed_));
		camera_->MoveXY(NcmEasing::GetValue(player_camera_speed_));
	}

	// クリア遷移
	if (ene_list_->NoticeEmpty())
	{
		is_clear_ = true;

		if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			SceneManager::SetNextScene(SceneName::TITLE);
		}
	}

	// ゲームオーバー遷移
	if (IsZeroOrLess(player_->GetHp()))
	{
		is_failed_ = true;

		if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			SceneManager::SetNextScene(SceneName::TITLE);
		}
	}

	// 各オブジェクト更新処理
	player_->Update();
	ene_list_->Update();
	grid_->Update();
	missile_mgr_->Update();
	lockon_sys_->Update();
	/*for (auto &i : grid_floor_)
	{
		i.Update();
	}*/

	// ミサイル追尾処理
	missile_mgr_->HomingTarget(*ene_list_);

	// 当たり判定
	CollisionProcess();

	// 塵描画他
	if (draw_dust_) { dust_->GenerateParticle(); }
	sky_dome_->Update();

	NcmUi::CalcBarSize(player_->GetChargeCount(), player_->GetMaxChargeTime());
}

void MainScene::Draw()
{
	using enum PipelineName;

	PreDraw::PreRender(Line);
	/*for (auto &i : grid_floor_)
	{
		i.Draw();
	}*/
	grid_->Draw();

	PreDraw::PreRender(Object3d_WireFrame);
	player_->Draw();
	ene_list_->Draw();
	missile_mgr_->Draw();
	lockon_sys_->Draw();
	if (draw_dust_) { dust_->Draw(); }

	if (is_wire_) { PreDraw::PreRender(Object3d); }
	sky_dome_->Draw();

	if (draw_coll_)
	{
		PreDraw::PreRender(Object3d_WireFrame);
		player_->DrawColl();
		ene_list_->DrawColl();
		missile_mgr_->DrawColl();
	}

	PreDraw::PreRender(Sprite);
	if (is_clear_)
	{
		if (CheckDoDisplay())
		{
			NcmSprite::DrawTex(space_);
		}

		NcmSprite::DrawTex(clear_);
	}
	else if (is_failed_)
	{
		if (CheckDoDisplay())
		{
			NcmSprite::DrawTex(space_);
		}

		NcmSprite::DrawTex(over_);
	}

	NcmUi::DrawMissileNumSet(LockOnSystem::GetCurrentTgtNum());
	NcmUi::DrawHp(player_->GetHp(), 30.0f);
	NcmUi::DrawSpace();
}

void MainScene::DebugDraw()
{
	player_->DebugDraw();
	lockon_sys_->DebugDraw();

	if (ImGui::CollapsingHeader("Draw"))
	{
		ImGui::Checkbox("DrawWireFrame?", &is_wire_);
		ImGui::Checkbox("DrawDust?", &draw_dust_);
		ImGui::Checkbox("DrawCollision?", &draw_coll_);
		ImGui::Checkbox("DrawNumbers?", &draw_numbers_);
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

	numbers_->DebugDraw();

	camera_->DebugDraw();
	//ene_list_->DebugDraw();

	NcmImGui::DragFloat2("UI_pos", ImGui_Ui_pos_, 1.0f, 0.0f, 1280.0f);
}

void MainScene::CollisionProcess()
{
	for (UINT i = 0; i < ene_list_->GetEnemies().size(); i++)
	{
		for (UINT j = 0; j < missile_mgr_->GetMissileList().size(); j++)
		{
			if (Collision::CheckSphere2Sphere(ene_list_->GetCollData(i), missile_mgr_->GetCollData(j)))
			{
				// ミサイルが有効なら
				if (missile_mgr_->GetIsValidity(j))
				{
					ene_list_->Death(i);
					missile_mgr_->Death(j);
				}
			}
		}
	}

	for (UINT i = 0; i < ene_list_->GetEnemies().size(); i++)
	{
		std::vector<Bullet> *bullets = &ene_list_->GetEnemies()[i].GetBulletList()->GetBullets();

		for (UINT j = 0; j < bullets->size(); j++)
		{
			if (Collision::CheckSphere2Sphere(player_->GetCollData(),
				MoveIterator(bullets->begin(), j)->GetCollData()))
			{
				player_->TakeDamage();
				MoveIterator(bullets->begin(), j)->Death();
			}
		}
	}
}

bool MainScene::CheckDoDisplay()
{
	int visible_time = 75;
	int invisivle_time = 25;
	static int visi_count = visible_time;
	static int invi_count = invisivle_time;

	if (visi_count >= 0)
	{
		visi_count--;
		return true;
	}
	else
	{
		invi_count--;

		if (invi_count <= 0)
		{
			visi_count = visible_time;
			invi_count = invisivle_time;
		}

		return false;
	}

	return false;
}
