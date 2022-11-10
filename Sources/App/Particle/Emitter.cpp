#include "Emitter.h"
#include <cmath>
#include "../Utility/NcmUtil.h"
#include "../Debug/NcmImGui.h"
#include "../../Lib/PreDraw/PreDraw.h"

using namespace DirectX;

std::unique_ptr<Model> Emitter::model_ = nullptr;
ncm_thandle Emitter::tex_handle_ = 0;

ParticleDesc Emitter::GenerateValue(const EmitterDesc &emitter)
{
	using namespace NcmUtill;

	EmitterDesc emi{};
	emi = emitter;

	{
		// マイナス防止
		emi.pos_rand_.x = fabs(emi.pos_rand_.x);
		emi.pos_rand_.y = fabs(emi.pos_rand_.y);
		emi.pos_rand_.z = fabs(emi.pos_rand_.z);

		// 位置のランダム値を取得
		XMFLOAT3 pos_rand = GenerateRandom((emi.pos_rand_ / 2) - emi.pos_rand_, emi.pos_rand_ / 2);

		// ランダム値を位置に加算
		emi.particle.position_ = emi.particle.position_ + pos_rand;
	}

	{
		// マイナス防止
		emi.vel_rand_.x = fabs(emi.vel_rand_.x);
		emi.vel_rand_.y = fabs(emi.vel_rand_.y);
		emi.vel_rand_.z = fabs(emi.vel_rand_.z);

		// 速度のランダム値を取得
		XMFLOAT3 vel_rand = GenerateRandom((emi.vel_rand_ / 2) - emi.vel_rand_, emi.vel_rand_ / 2);

		// ランダム値を速度に加算
		emi.particle.velocity_ = emi.particle.velocity_ + vel_rand;
	}

	XMFLOAT3 acc{};
	acc.y = 0.000f;

	// 設定された値で要素を作成
	ParticleDesc p{};
	p.position_ = emi.particle.position_;
	p.velocity_ = emi.particle.velocity_;
	p.accel_ = acc;
	p.scale_ = p.s_scale_ = emi.particle.s_scale_;
	p.e_scale_ = 0.0f;
	p.life_ = emi.particle.life_;
	p.tex_handle_ = tex_handle_;

	return p;
}

void Emitter::Add(const ParticleDesc &p)
{
	// 先頭に要素を構築
	particles_.emplace_front();
	// 初期化
	particles_.front().Initialize(model_.get(), p);
}

void Emitter::LoadResources()
{
	if (!model_)
	{
		// モデルデータの読み込みと生成
		model_ = make_unique<Model>();
		model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");

		tex_handle_ = NcmSprite::LoadTex(L"Resources/textures/effect1.png");
	}
}

void Emitter::GenerateParticle()
{
	// 寿命を使用する設定なら
	if (emitter_desc_.use_life_ && emitter_desc_.life_ > 0)
	{
		// 寿命を削る
		emitter_desc_.life_--;
	}

	// 寿命を迎えていなければ && 終了命令が来ていなければ
	if (emitter_desc_.life_ > 0 && !emitter_desc_.is_dead_)
	{
		// 1フレーム中に指定された数を生成する
		for (UINT count = emitter_desc_.gene_num_; count > 0; count--)
		{
			Add(GenerateValue(emitter_desc_));
		}
	}

	// 寿命が尽きた粒をコンテナから削除
	particles_.remove_if([](Particle &x) { return x.GetIsDead(); });

	// 全てのパーティクルを更新
	for (auto &i : particles_)
	{
		i.Update();
	}
}

void Emitter::PrepareTerminate()
{
	// 終了フラグが立っていなかったら
	if (!emitter_desc_.is_dead_)
	{
		// 終了フラグを立たせる
		emitter_desc_.is_dead_ = true;
	}
}

bool Emitter::NoticeCanTerminate()
{
	// パーティクルコンテナが空になったら
	if (particles_.empty())
	{
		// 終了準備が出来たと知らせる
		return true;
	}

	return false;
}

void Emitter::Draw()
{
	//PreDraw::PreRender(PipelineName::PlatePoly);

	// 全てのパーティクルを描画
	for (auto &i : particles_)
	{
		i.Draw();
	}
}

void Emitter::DebugDraw()
{
	NcmImGui::DragFloat3("pos", emitter_desc_.particle.position_, 1.0f, -100.0f, 100.0f);
	NcmImGui::SliderFloat3("range", emitter_desc_.pos_rand_, 0.0f, 100.0f);
	NcmImGui::DragFloat3("velocity", emitter_desc_.particle.velocity_, 0.1f, -5.0f, 5.0f);
	NcmImGui::SliderFloat3("vel_range", emitter_desc_.vel_rand_, 0.0f, 1.0f);
	ImGui::DragFloat("scale", &emitter_desc_.particle.s_scale_, 0.1f, 0.1f, 50.0f);
	ImGui::DragInt("life", &emitter_desc_.particle.life_, 1.0f, 0, 1000);
	NcmImGui::SliderUINT("num", emitter_desc_.gene_num_, 0, 50);
	ImGui::Text("life : %d", emitter_desc_.life_);
	ImGui::Spacing();
	ImGui::Spacing();
}
