#include "MainScene.h"
#include <vector>
#include "../../App/Missile/MissileLauncher.h"
#include "../../Lib/Collision/Collision.h"
#include "../../Lib/Math/Easing/NcmEasing.h"
#include "../../Lib/Input/NcmInput.h"
#include "../../Lib/DirectX/PreDraw.h"
#include "../../Lib/Input/KeyboardInput.h"
#include "../../Lib/PostEffect/PostEffect.h"
#include "../../Lib/Utility/NcmUtil.h"
#include "../../Lib/DebugUtill/NcmDebug.h"
#include "../Scene/SceneManager/SceneManager.h"

using namespace NcmUtill;

MainScene::MainScene() :
	camera_(std::make_unique<Camera>()),
	player_(std::make_unique<Player>()),
	enemy_(std::make_unique<Enemy>()),
	ene_list_(std::make_unique<EnemiesList>()),
	grid_floor_(std::make_unique<NcmGridFloor>()),
	missile_mgr_(std::make_unique<MissileManager>()),
	dust_(make_unique<Emitter>()),
	sky_dome_(make_unique<Object3d>()),
	model_sky_dome_(make_unique<Model>()),
	reticle_(make_unique<Reticle>()),
	lockon_sys_(make_unique<LockOnSystem>()),
	numbers_(make_unique<Numbers>()),
	ui_(make_unique<NcmUi>()),
	ult_(make_unique<UltimateManager>()),
	particle_mgr_(make_unique<NcmParticleManager>()),
	texture_(),
	clear_(),
	over_(),
	space_(),
	key_bind_(0),
	is_wire_(true),
	draw_dust_(false),
	draw_coll_(false),
	draw_numbers_(false),
	is_clear_(false),
	is_failed_(false),
	ImGui_detection_range_(1000.0f),
	ImGui_Ui_pos_(Win32App::FCENTER_),
	player_speed_(),
	player_dec_speed_(),
	fov_acc_value_(),
	fov_dec_value_(),
	is_scene_change_(),
	result_alpha_(0.0f)
{
	// テクスチャのロード
	clear_ = NcmSprite::LoadTex("Resources/Textures/clear.png");
	over_ = NcmSprite::LoadTex("Resources/Textures/SC/over.png");
	space_ = NcmSprite::LoadTex("Resources/Textures/space.png");

	// 各リソースのロード
	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");

	Player::LoadResources();
	Enemy::LoadResources();
	Emitter::LoadResources();
	Missile::LoadResources();
	Reticle::LoadResources();
	LockOnSystem::LoadResources();
	NcmUi::LoadResources();
	UltimateManager::LoadResources();
}

MainScene::~MainScene()
{
	NcmSprite::TermSprite();
}

void MainScene::Initialize()
{
	Object3d::SetCamera(camera_.get());
	GridRender::SetCamera(camera_.get());
	PlatePoly::SetCamera(camera_.get());
	NcmPlatePoly::SetCamera(camera_.get());
	NcmGridFloor::SetCamera(camera_.get());

	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, CAM_INIT_POS_.z));
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
	player_->Initialize(lockon_sys_.get(), ult_.get(), INIT_POS_);
	player_->SetCamPtr(camera_.get());
	missile_mgr_->Initialize(player_.get(), lockon_sys_.get());
	grid_floor_->Initialize(XMFLOAT3(), 10);
	reticle_->Initialize();
	numbers_->Initialize();
	ult_->Initialize();
	MissileLauncher::SetPtr(missile_mgr_.get(), lockon_sys_.get());
	Missile::SetPtr(player_.get());

	// enemyの生成
	ene_list_->AddTemplateSet();

	// 塵エミッターの生成
	EmitterDesc p;
	p.part_desc_.position_ = { 0.0f, 0.0f, 0.0f };
	p.part_desc_.velocity_ = { 0.0f, 0.0f, 0.0f };
	p.part_desc_.accel_ = { 0, 0.001f, 0 };
	p.part_desc_.life_ = 100;
	p.part_desc_.s_scale_ = 0.3f;
	p.pos_rand_ = { 1000.0f, 1000.0f, 1000.0f };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 2;
	p.use_life_ = false;
	dust_->SetEmitterDesc(p);

	particle_mgr_->Initialize();

	// スプライトの初期化
	NcmSprite::SetSize(clear_, { 1280.0f, 720.0f });
	NcmSprite::SetSize(over_, { 1280.0f, 720.0f });

	XMINT2 pos = { Win32App::CENTER_.x, 650 };
	XMFLOAT2 size = NcmSprite::GetSize(space_);
	NcmSprite::SetPos(space_, pos);
	NcmSprite::SetSize(space_, { size.x / 2, size.y / 2 });
	NcmSprite::SetAnchorPoint(space_, { 0.5f, 0.5f });

	// チートモードをONにする
	// ・自機の無敵
	// ・ULTの即時発射
	NcmDebug::GetInstance()->SetCheatMode(false);

	// キーバインド機能を使用するか
#ifdef _DEBUG
	NcmDebug::GetInstance()->SetDebugMode(true);
	key_bind_ = (int)(KeyBind::Player);
#else
	NcmDebug::GetInstance()->SetDebugMode(false);
#endif

	is_clear_ = false;

	// イージング情報の設定
	NcmEaseDesc ease;
	ease.ease_type = NcmEaseType::OutCirc;
	ease.init_value = 0.5f;
	ease.total_move = SPEED_;
	ease.t_rate = 0.05f;
	player_speed_ = NcmEasing::RegisterEaseData(ease);

	ease.ease_type = NcmEaseType::OutCirc;
	ease.init_value = SPEED_;
	ease.total_move = -0.5f;
	ease.t_rate = 0.05f;
	player_dec_speed_ = NcmEasing::RegisterEaseData(ease);

	ease.ease_type = NcmEaseType::OutCubic;
	ease.init_value = NORMAL_FOV_;
	ease.total_move = ACCEL_FOV_ - NORMAL_FOV_;
	ease.t_rate = 0.05f;
	fov_acc_value_ = NcmEasing::RegisterEaseData(ease);

	ease.ease_type = NcmEaseType::OutCubic;
	ease.init_value = ACCEL_FOV_;
	ease.total_move = NORMAL_FOV_ - ACCEL_FOV_;
	ease.t_rate = 0.05f;
	fov_dec_value_ = NcmEasing::RegisterEaseData(ease);

	// UI関連の初期化
	NcmUi::Initialize();

	is_scene_change_ = false;
}

void MainScene::Finalize()
{}

void MainScene::Update()
{
	// 画面遷移を実行
	if (!is_scene_change_)
	{
		SceneManager::OutChangeScene(SCENE_CHANGE_SPEED_);
	}

	// パーティクルの全要素を削除
	NcmParticleManager::ClearParticleArgsBeforeUpdate();

	if (NcmDebug::GetInstance()->IsDebugMode())
	{
		// キーバインドごとの操作
		if (key_bind_ == (int)(KeyBind::Camera))
		{
			camera_->BasicCameraMoveTrack(2.0f);
		}
		else if (key_bind_ == (int)(KeyBind::Player))
		{
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
			camera_->SetEye(XMFLOAT3(0, 10.0f, -10.0f));
			player_->Initialize(lockon_sys_.get(), ult_.get(), INIT_POS_);
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

	// クリア遷移
	if (ene_list_->NoticeEmpty())
	{
		is_clear_ = true;

		if (KeyboardInput::TriggerKey(DIK_SPACE) ||
			NcmInput::IsTrigger(NcmButtonType::A) ||
			NcmInput::IsTrigger(NcmButtonType::B) ||
			NcmInput::IsTrigger(NcmButtonType::X) ||
			NcmInput::IsTrigger(NcmButtonType::Y))
		{
			is_scene_change_ = true;
		}
	}

	// ゲームオーバー遷移
	if (IsZeroOrLess(player_->GetHp()))
	{
		is_failed_ = true;

		if (KeyboardInput::TriggerKey(DIK_SPACE) ||
			NcmInput::IsTrigger(NcmButtonType::A) ||
			NcmInput::IsTrigger(NcmButtonType::B) ||
			NcmInput::IsTrigger(NcmButtonType::X) ||
			NcmInput::IsTrigger(NcmButtonType::Y))
		{
			is_scene_change_ = true;
		}
	}

	if (is_scene_change_)
	{
		if (SceneManager::InChangeScene(SCENE_CHANGE_SPEED_))
		{
			SceneManager::SetNextScene(SceneName::TITLE);
		}
	}

	// 各オブジェクト更新処理
	camera_->FollowZCameraMove(*player_);
	camera_->Update();
	player_->Update();
	ene_list_->Update();
	grid_floor_->FollowVertLineForPlayer(player_->GetPos().z);
	grid_floor_->Update();
	missile_mgr_->Update();
	lockon_sys_->Update();
	ult_->Update();
	particle_mgr_->Update();

	// ミサイル追尾処理
	missile_mgr_->UpdateTargetPos(*ene_list_);
	missile_mgr_->HomingTarget();

	// 当たり判定
	CollisionProcess();

	// 塵描画他
	if (draw_dust_)
	{
		dust_->GenerateParticle();
		dust_->UpdateParticle();
	}

	sky_dome_->SetPos(XMFLOAT3(0, 0, player_->GetPos().z));
	sky_dome_->Update();

	NcmUi::CalcBarSize(player_->GetChargeCount(), player_->GetMaxChargeTime());

	// C が押されたら
	if (KeyboardInput::TriggerKey(DIK_C))
	{
		// モードを切り替える
		bool mode = NcmDebug::GetInstance()->IsCheatMode();
		NcmDebug::GetInstance()->SetCheatMode(!mode);
	}
}

void MainScene::Draw()
{
	using enum PipelineName;

	PreDraw::SetPipeline(Line);
	grid_floor_->Draw();

	PreDraw::SetPipeline(Object3d_WireFrame);
	player_->Draw();
	ene_list_->Draw();
	missile_mgr_->Draw();
	lockon_sys_->Draw();
	if (draw_dust_) { dust_->Draw(); }

	if (is_wire_) { PreDraw::SetPipeline(Object3d); }
	sky_dome_->Draw();

	if (draw_coll_)
	{
		PreDraw::SetPipeline(Object3d_WireFrame);
		player_->DrawColl();
		ene_list_->DrawColl();
		missile_mgr_->DrawColl();
	}

	PreDraw::SetPipeline(PlatePoly);
	particle_mgr_->Draw();	
}

void MainScene::AfterPostEffectDraw()
{
	using enum PipelineName;

	PreDraw::SetPipeline(Sprite);
	// クリアしたら
	if (is_clear_)
	{
		AlphaTransition(0.05f);

		// αをセット
		NcmSprite::SetAlpha(space_, result_alpha_);
		NcmSprite::SetAlpha(clear_, result_alpha_);

		// 点滅表示
		if (CheckDoDisplay())
		{
			NcmSprite::DrawTex(space_);
		}

		NcmSprite::DrawTex(clear_);
	}
	// 失敗したら
	else if (is_failed_)
	{
		AlphaTransition(0.05f);

		// αをセット
		NcmSprite::SetAlpha(space_, result_alpha_);
		NcmSprite::SetAlpha(over_, result_alpha_);

		// 点滅表示
		if (CheckDoDisplay())
		{
			NcmSprite::DrawTex(space_);
		}

		NcmSprite::DrawTex(over_);
	}

	NcmUi::DrawMissileNumSet(LockOnSystem::GetCurrentTgtNum());
	NcmUi::DrawHp(player_->GetHp(), 30.0f);
	ene_list_->DrawRemainEnemyNum();

	if (NcmDebug::GetInstance()->IsCheatMode())
	{
		NcmUi::DrawCheatGuide();
	}
	else
	{
		ult_->DrawUi();
	}
}

void MainScene::DebugDraw()
{
	bool debug = NcmDebug::GetInstance()->IsDebugMode();
	ImGui::Checkbox("DebugMode?", &debug);
	NcmDebug::GetInstance()->SetDebugMode(debug);

	//grid_floor_->DebugDraw();

	NcmParticleManager::StaticDebugDraw();
	Emitter::StaticDebugDraw();

	ene_list_->DebugDraw();

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

	NcmImGui::DragFloat2("UI_pos", ImGui_Ui_pos_, 1.0f, 0.0f, 1280.0f);
	ult_->DebugDraw();

	NcmInput::DebugDraw();
}

void MainScene::CollisionProcess()
{
	// 敵とミサイル
	for (UINT i = 0; i < ene_list_->GetEnemies().size(); i++)
	{
		for (UINT j = 0; j < missile_mgr_->GetMissileList().size(); j++)
		{
			if (Collision::CheckSphere2Sphere(ene_list_->GetCollData(i), missile_mgr_->GetCollData(j)))
			{
				HitSequence(i, j);
			}
		}
	}

	// 敵ミサイルと自機
	for (UINT i = 0; i < missile_mgr_->GetMissileList().size(); i++)
	{
		// そのミサイルのターゲットがプレイヤーなら
		if (missile_mgr_->GetTgtID(i) == (int32_t)(TargetIs::Player))
		{
			if (Collision::CheckSphere2Sphere(player_->GetCollData(), missile_mgr_->GetCollData(i)))
			{
				player_->TakeDamage();
				missile_mgr_->Death(i);
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
}

void MainScene::AlphaTransition(float trans_speed)
{
	// α値が1以下なら
	if (result_alpha_ <= 1.0f)
	{
		// αを1に近づけていく
		result_alpha_ += trans_speed;
	}

	// α値が1以上なら
	if (result_alpha_ >= 1.0f)
	{
		// 値を1に固定する
		result_alpha_ = 1.0f;
	}
}

void MainScene::HitSequence(int i, int j)
{
	// ミサイルが有効なら
	if (missile_mgr_->GetIsValidity(j) && missile_mgr_->GetTgtID(j) != (int32_t)(TargetIs::Player))
	{
		if (missile_mgr_->Death(j))
		{
			if (NcmDebug::GetInstance()->IsCheatMode())
			{
				return;
			}

			ene_list_->Death(i);
			ult_->AddUltValue(20);
		}
	}
}
