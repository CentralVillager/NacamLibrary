#include "MainScene.h"
#include "../../Lib/PreDraw/PreDraw.h"
#include "../../Lib/Input/KeyboardInput.h"
#include "../Scene/SceneManager/SceneManager.h"
#include "../Sources/App/Collision/Collision.h"

MainScene::MainScene()
{
	// �e�I�u�W�F�N�g�̐���
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
	numbers_ = make_unique<Numbers>();

	// �e�N�X�`���̃��[�h
	clear_ = Sprite::LoadTex(L"Resources/Textures/clear.png");
	space_ = Sprite::LoadTex(L"Resources/Textures/space.png");

	// �e���\�[�X�̃��[�h
	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");
	Player::LoadResources();
	Enemy::LoadResources();
	Emitter::LoadResources();
	Missile::LoadResources();
	Reticle::LoadResources();
	LockOnSystem::LoadResources();
	Numbers::LoadResources();
}

MainScene::~MainScene()
{}

void MainScene::Initialize()
{
	Object3d::SetCamera(camera_.get());
	GridRender::SetCamera(camera_.get());

	// �J�����̏�����
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, 0));
	camera_->MoveEye(XMFLOAT3(0, 10.0f, 0));
	camera_->Update();

	sky_dome_->Initialize();
	sky_dome_->SetModel(model_sky_dome_.get());
	sky_dome_->SetScale(50.0f);
	sky_dome_->Update();

	lockon_sys_->Initialize(player_.get(), ene_list_.get(), 1);
	missile_mgr_->Initialize(lockon_sys_.get());
	player_->Initialize(missile_mgr_.get(), lockon_sys_.get());
	grid_->Initialize(200, 10, XMFLOAT3(0, 0, 0));
	reticle_->Initialize();
	numbers_->Initialize();
	for (UINT i = 0; i < grid_floor_.size(); i++)
	{
		grid_floor_[i].Initialize(200, 10, XMFLOAT3(0, 0, i * 1000));
	}

	// enemy�̐���
	ene_list_->AddTemplateSet();

	// �o�G�~�b�^�[�̐���
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

	// �L�[�o�C���h�@�\���g�p���邩
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
		// �L�[�o�C���h���Ƃ̑���
		if (key_bind_ == (int)(KeyBind::Camera))
		{
			camera_->BasicCameraMoveTrack(2.0f);
		}
		else if (key_bind_ == (int)(KeyBind::Player))
		{
			// �~�T�C���̔���
			/*if (KeyboardInput::TriggerKey(DIK_SPACE))
			{
				player_->FireMissile();
			}

			if (KeyboardInput::TriggerKey(DIK_T))
			{
				lockon_sys_->AddTargetNum();
			}*/

			player_->Move(1.0f);
		}
		else if (key_bind_ == (int)(KeyBind::AddEnemy))
		{
			if (KeyboardInput::TriggerKey(DIK_SPACE))
			{
				ene_list_->Add(XMFLOAT3(0, 0, 100));
			}
		}

		// ���Z�b�g
		if (KeyboardInput::TriggerKey(DIK_2))
		{
			//camera_->MoveCameraTrack(XMFLOAT3(0, 10.0f, 0));
			camera_->SetEye(XMFLOAT3(0, 20.0f, -20.0f));
			camera_->SetTarget(XMFLOAT3(0, 10.0f, 0));
			player_->Initialize(missile_mgr_.get(), lockon_sys_.get());
		}

		// �L�[�A�T�C���̕ύX
		if (KeyboardInput::TriggerKey(DIK_RETURN))
		{
			key_bind_++;

			if (key_bind_ >= (int)(KeyBind::MaxNum))
			{
				key_bind_ = (int)(KeyBind::Start);
				key_bind_++;
			}
		}

		// �����V�[���J��
		if (KeyboardInput::TriggerKey(DIK_0))
		{
			SceneManager::SetNextScene(SceneName::RESULT);
		}
	}
	else if (!use_keybind_)
	{
		// �~�T�C������
		/*if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			player_->FireMissile();
		}*/
	}

	// �������U���g
	if (KeyboardInput::TriggerKey(DIK_0))
	{
		is_result_ = true;
	}

	// �N���A�J��
	if (ene_list_->NoticeEmpty())
	{
		is_result_ = true;

		if (KeyboardInput::TriggerKey(DIK_SPACE))
		{
			SceneManager::SetNextScene(SceneName::TITLE);
		}
	}

	player_->MoveXY(1.0f);

	for (auto &i : grid_floor_)
	{
		i.MoveMinusZ();
	}

	// �e�I�u�W�F�N�g�X�V����
	player_->Update();
	ene_list_->Update();
	grid_->Update();
	missile_mgr_->Update();
	lockon_sys_->Update();
	for (auto &i : grid_floor_)
	{
		i.Update();
	}

	// �~�T�C���ǔ�����
	missile_mgr_->HomingTarget(*ene_list_);

	// �����蔻��
	CollisionProcess();

	// �o�`�摼
	if (draw_dust_) { dust_->GenerateParticle(); }
	sky_dome_->Update();
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

	if (!is_wire_) { PreDraw::PreRender(Object3d); }
	sky_dome_->Draw();

	if (draw_coll_)
	{
		PreDraw::PreRender(Object3d_WireFrame);
		player_->DrawColl();
		ene_list_->DrawColl();
		missile_mgr_->DrawColl();
	}

	PreDraw::PreRender(Sprite);
	if (is_result_)
	{
		if (CheckDoDisplay())
		{
			Sprite::DrawTex(space_);
		}

		Sprite::DrawTex(clear_);
	}

	using enum HorizontalAlignment;
	using enum VerticalAlignment;
	lockon_sys_->DrawNum();
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
}

void MainScene::CollisionProcess()
{
	for (UINT i = 0; i < ene_list_->GetEnemies().size(); i++)
	{
		for (UINT j = 0; j < missile_mgr_->GetMissileList().size(); j++)
		{
			if (Collision::CheckSphere2Sphere(ene_list_->GetCollData(i), missile_mgr_->GetCollData(j)))
			{
				// �~�T�C�����L���Ȃ�
				if (missile_mgr_->GetIsValidity(j))
				{
					ene_list_->Death(i);
					missile_mgr_->Death(j);
				}
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
