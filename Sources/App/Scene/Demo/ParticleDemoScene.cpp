#include "ParticleDemoScene.h"
#include "../../DebugUtill/NcmImGui.h"
#include "../../Scene/SceneManager/SceneManager.h"
#include "../../../Lib/PreDraw/PreDraw.h"
#include "../../../Lib/Input/KeyboardInput.h"

ParticleDemoScene::ParticleDemoScene()
{
	// カメラの生成
	camera_ = make_unique<Camera>();

	sky_dome_ = make_unique<Object3d>();
	model_sky_dome_ = make_unique<Model>();
	model_sky_dome_->LoadObjModel("Resources/SkyDome/", "skydome.obj", "skydome.mtl");

	// パーティクルデータの生成
	contrail_1_ = make_unique<Emitter>();
	contrail_2_ = make_unique<Emitter>();
	emitter_1_ = make_unique<Emitter>();

	// パーティクルコンテナの初期化
	contrails_1_.resize(0);

	dust_ = make_unique<Emitter>();
	particle_mgr_ = make_unique<NcmParticleManager>();
}

ParticleDemoScene::~ParticleDemoScene()
{}

void ParticleDemoScene::Initialize()
{
	// カメラの初期化
	camera_->Initialize();
	camera_->SetDistance(20.0f);
	Object3d::SetCamera(camera_.get());

	sky_dome_->Initialize();
	sky_dome_->SetModel(model_sky_dome_.get());
	sky_dome_->SetPos({ 0, 0, 0 });
	sky_dome_->SetScale({ 1.0f, 1.0f, 1.0f });
	sky_dome_->SetRot({ 0.0f, 0, 0 });
	sky_dome_->Update();

	Emitter::LoadResources();

	particle_mgr_->Initialize();

	mode_ = MODE::MONO;

	// エミッターの初期化
	EmitterDesc p;
	p.part_desc_.position_ = { 0.0f, 0.0f, 10.0f };
	p.part_desc_.velocity_ = { 0.0f, 0.0f, 0.0f };
	p.part_desc_.accel_ = { 0, 0.001f, 0 };
	p.part_desc_.life_ = 100;
	p.part_desc_.s_scale_ = 1.0f;
	p.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	emitter_1_->SetEmitterDesc(p);

	p.part_desc_.position_ = { 10.0f, 0.0f, -50.0f };
	p.part_desc_.velocity_ = { 0.0f, 0.0f, -1.0f };
	p.part_desc_.accel_ = { 0, 0.001f, 0 };
	p.part_desc_.life_ = 100;
	p.part_desc_.s_scale_ = 1.0f;
	p.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	contrail_1_->SetEmitterDesc(p);

	p.part_desc_.position_ = { -10.0f, 0.0f, -50.0f };
	p.part_desc_.velocity_ = { 0.0f, 0.0f, -1.0f };
	p.part_desc_.accel_ = { 0, 0.001f, 0 };
	p.part_desc_.life_ = 100;
	p.part_desc_.s_scale_ = 1.0f;
	p.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	contrail_2_->SetEmitterDesc(p);

	emitter_life_ = 100;

	p.part_desc_.position_ = { 0.0f, 0.0f, 0.0f };
	p.part_desc_.velocity_ = { 0.0f, 0.0f, 0.0f };
	p.part_desc_.accel_ = { 0, 0.001f, 0 };
	p.part_desc_.life_ = 100;
	p.part_desc_.s_scale_ = 0.1f;
	p.pos_rand_ = { 200.0f, 200.0f, 200.0f };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	dust_->SetEmitterDesc(p);
}

void ParticleDemoScene::Finalize()
{}

void ParticleDemoScene::Update()
{
	NcmParticleManager::ClearParticleArgsBeforeUpdate();

	// カメラ操作
	if (KeyboardInput::PushKey(DIK_W) || KeyboardInput::PushKey(DIK_S) || KeyboardInput::PushKey(DIK_D) || KeyboardInput::PushKey(DIK_A) || KeyboardInput::PushKey(DIK_R) || KeyboardInput::PushKey(DIK_F))
	{

		if (KeyboardInput::PushKey(DIK_W)) { camera_->MoveCameraTrack({ 0.0f,+1.0f,0.0f }); }
		else if (KeyboardInput::PushKey(DIK_S)) { camera_->MoveCameraTrack({ 0.0f,-1.0f,0.0f }); }

		if (KeyboardInput::PushKey(DIK_D)) { camera_->MoveCameraTrack({ +1.0f, 0.0f, 0.0f }); }
		else if (KeyboardInput::PushKey(DIK_A)) { camera_->MoveCameraTrack({ -1.0f,0.0f,0.0f }); }

		if (KeyboardInput::PushKey(DIK_R)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, +1.0f }); }
		else if (KeyboardInput::PushKey(DIK_F)) { camera_->MoveCameraTrack({ 0.0f, 0.0f, -1.0f }); }
	}

	if (KeyboardInput::PushKey(DIK_UP)) { camera_->MoveEye({ 0.0f, -1.0f, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_DOWN)) { camera_->MoveEye({ 0.0f, +1.0f, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_LEFT)) { camera_->MoveEye({ +1.0f, 0.0f, 0.0f }); }
	if (KeyboardInput::PushKey(DIK_RIGHT)) { camera_->MoveEye({ -1.0f, 0.0f, 0.0f }); }

	sky_dome_->Update();

	// カメラ位置をリセット
	if (notice_reset_)
	{
		camera_->SetEye({ 0.0f, 0.0f, -camera_->GetDistance() });
		camera_->SetTarget({ 0.0f, 0.0f, 0.0f });
	}
	camera_->Update();

	switch (mode_)
	{
	case MODE::MONO:

		// 全パラメータをリセット
		if (notice_reset_)
		{
			ResetParam();
		}

		emitter_1_->GenerateParticle();
		emitter_1_->UpdateParticle();
		break;

	case MODE::MISSILE:

		// 位置をリセット
		if (notice_reset_)
		{
			ResetPos();
		}

		// z方向に進める
		MoveZ();

		// パーティクルを生成
		contrail_1_->GenerateParticle();
		contrail_2_->GenerateParticle();
		break;

	case MODE::MISSILE_CONTAINER:

		// パーティクルを生成
		if (notice_generate_)
		{
			contrails_1_.emplace_front();
			Emitter *emitter = &contrails_1_.front();

			EmitterDesc p;
			p.part_desc_.position_ = { 10.0f, 0.0f, -50.0f };
			p.part_desc_.velocity_ = { 0.0f, 0.f, -1.0f };
			p.part_desc_.accel_ = { 0, 0, 0 };
			p.part_desc_.life_ = 100;
			p.part_desc_.s_scale_ = 1.0f;
			p.pos_rand_ = { 0.0f, 0.0f, 0.0f };
			p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
			p.gene_num_ = 1;
			p.use_life_ = true;
			p.life_ = emitter_life_;
			emitter->SetEmitterDesc(p);

			contrails_1_.emplace_front();
			emitter = &contrails_1_.front();
			p.part_desc_.position_ = { -10.0f, 0.0f, -50.0f };
			p.part_desc_.velocity_ = { 0.0f, 0.f, -1.0f };
			p.part_desc_.accel_ = { 0, 0, 0 };
			p.part_desc_.life_ = 100;
			p.part_desc_.s_scale_ = 1.0f;
			p.pos_rand_ = { 0.0f, 0.0f, 0.0f };
			p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
			p.gene_num_ = 1;
			p.use_life_ = true;
			p.life_ = emitter_life_;
			emitter->SetEmitterDesc(p);
		}

		for (auto &i : contrails_1_)
		{
			// z方向に移動
			XMFLOAT3 pos = i.GetPosition();
			pos.z += 1.0f;
			i.SetPosition(pos);

			i.GenerateParticle();
			i.UpdateParticle();
		}

		contrails_1_.remove_if([](Emitter &x) { return x.NoticeCanTerminate(); });
		break;

	default:
		break;
	}

	notice_reset_ = false;
	notice_generate_ = false;

	dust_->GenerateParticle();
	dust_->UpdateParticle();
	particle_mgr_->Update();
}

void ParticleDemoScene::Draw()
{
	if (is_wire_)
	{
		PreDraw::SetPipeline(PipelineName::Object3d_WireFrame);
	}
	else
	{
		PreDraw::SetPipeline(PipelineName::Object3d);
	}
	sky_dome_->Draw();

	// モード毎にメニュー切り替え
	switch (mode_)
	{
	case MODE::MONO:

		emitter_1_->Draw();
		break;

	case MODE::MISSILE:

		contrail_1_->Draw();
		contrail_2_->Draw();
		break;

	case MODE::MISSILE_CONTAINER:

		for (auto &i : contrails_1_)
		{

			i.Draw();
		}
		break;

	default:
		break;
	}

	dust_->Draw();
	particle_mgr_->Draw();
}

void ParticleDemoScene::DebugDraw()
{
	ImGui::Checkbox("WireFrame", &is_wire_);

	// モード選択
	int mode = static_cast<int>(mode_);
	ImGui::SliderInt("mode", &mode, 0, 2);
	mode_ = static_cast<MODE>(mode);

	// リセット
	if (ImGui::Button("RESET") && !notice_reset_)
	{
		notice_reset_ = true;
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	// モード毎にメニュー切り替え
	switch (mode_)
	{
	case MODE::MONO:

		ImGui::Text("Normal Particle Demo");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		emitter_1_->DebugDraw();

		break;

	case MODE::MISSILE:

		ImGui::Text("Missile");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		contrail_1_->DebugDraw();
		contrail_2_->DebugDraw();

		break;

	case MODE::MISSILE_CONTAINER:

		ImGui::Text("Many Missiles");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SliderInt("EmitterLife", &emitter_life_, 0, 500);

		if (ImGui::Button("GENERATE") && !notice_generate_)
		{
			notice_generate_ = true;
		}

		for (auto &i : contrails_1_)
		{
			i.DebugDraw();
		}

		break;

	default:
		break;
	}
}

void ParticleDemoScene::ResetParam()
{
	EmitterDesc p;
	p.part_desc_.position_ = { 0.0f, 0.0f, 10.0f };
	p.part_desc_.velocity_ = { 0.0f, 0.0f, 0.0f };
	p.part_desc_.accel_ = { 0, 0.001f, 0 };
	p.part_desc_.life_ = 100;
	p.part_desc_.s_scale_ = 1.0f;
	p.pos_rand_ = { 0.0f, 0.0f, 0.0f };
	p.vel_rand_ = { 0.1f, 0.1f, 0.1f };
	p.gene_num_ = 1;
	p.use_life_ = false;
	emitter_1_->SetEmitterDesc(p);
}

void ParticleDemoScene::ResetPos()
{
	contrail_1_->SetPosition({ 10.0f, 0.0f, -50.0f });
	contrail_2_->SetPosition({ -10.0f, 0.0f, -50.0f });
}

void ParticleDemoScene::MoveZ()
{
	XMFLOAT3 pos = contrail_1_->GetPosition();
	pos.z += 1.0f;
	contrail_1_->SetPosition(pos);

	pos = contrail_2_->GetPosition();
	pos.z += 1.0f;
	contrail_2_->SetPosition(pos);
}
