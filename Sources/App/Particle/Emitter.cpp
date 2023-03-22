#include "Emitter.h"
#include <cmath>
#include "../Utility/NcmUtil.h"
#include "../DebugUtill/NcmImGui.h"
#include "../../Lib/DirectX/PreDraw.h"
#include "../../App/Particle/NcmParticleManager.h"

using namespace DirectX;
using namespace NcmUtill;

std::unique_ptr<Model> Emitter::model_ = nullptr;
ncm_thandle Emitter::tex_handle_ = 0;
std::list<Particle> Emitter::shared_particles_;
uint32_t Emitter::particle_num_ = 0;

ParticleDesc Emitter::GenerateValue(const EmitterDesc &emitter)
{
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
		emi.part_desc_.position_ = emi.part_desc_.position_ + pos_rand;
	}

	{
		// マイナス防止
		emi.vel_rand_.x = fabs(emi.vel_rand_.x);
		emi.vel_rand_.y = fabs(emi.vel_rand_.y);
		emi.vel_rand_.z = fabs(emi.vel_rand_.z);

		// 速度のランダム値を取得
		XMFLOAT3 vel_rand = GenerateRandom((emi.vel_rand_ / 2) - emi.vel_rand_, emi.vel_rand_ / 2);

		// ランダム値を速度に加算
		emi.part_desc_.velocity_ = emi.part_desc_.velocity_ + vel_rand;
	}

	XMFLOAT3 acc{};
	acc.y = 0.000f;

	// 設定された値で要素を作成
	ParticleDesc p{};
	p.position_ = emi.part_desc_.position_;
	p.velocity_ = emi.part_desc_.velocity_;
	p.accel_ = acc;
	p.scale_ = p.s_scale_ = emi.part_desc_.s_scale_;
	p.e_scale_ = p.scale_;
	p.life_ = emi.part_desc_.life_;
	p.color_ = emi.part_desc_.color_;
	p.alpha_ = 1.0f;
	p.tex_handle_ = tex_handle_;
	p.is_used_ = true;

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

		tex_handle_ = NcmSprite::LoadTex("Resources/textures/particle/effect1.png");
	}
}

void Emitter::Update()
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
		// 最後尾の次の参照を取得
		auto itr = shared_particles_.end();

		// 1フレーム中に指定された数を使う
		for (UINT count = emitter_desc_.gene_num_; count > 0; count--)
		{
			UseParticle(itr);
			updated_dirty_ = true;
		}
	}

	// パーティクルが生成されていたら
	if (updated_dirty_)
	{
		// is_used_ フラグが立っている要素を前方へソート
		shared_particles_.sort([](Particle &lhs, Particle &rhs) { return lhs.GetIsUsed() > rhs.GetIsUsed(); });
		updated_dirty_ = false;
	}

	// 全てのパーティクルに対して
	for (auto &i : shared_particles_)
	{
		// 使われているなら
		if (i.GetIsUsed())
		{
			// 更新する
			i.Update();
		}
		// 使われていないなら
		else
		{
			// ループを抜ける
			break;
		}
		// 死んでいたら
		if (i.GetIsDead())
		{
			// 無効化する
			i.SetIsUsed(false);
			// 値をリセットする
			i.ResetParamater();
			particle_num_--;
		}
	}

	// is_used_ フラグが立っている要素を前方へソート
	shared_particles_.sort([](Particle &lhs, Particle &rhs) { return lhs.GetIsUsed() > rhs.GetIsUsed(); });
}

void Emitter::UseParticle(std::list<Particle>::iterator last_itr)
{
	// 最後尾から順に使用
	last_itr--;

	// その要素を有効化する
	last_itr->SetIsUsed(true);

	// パラメータを設定
	last_itr->SetParticleValue(GenerateValue(emitter_desc_));

	particle_num_++;
}

void Emitter::GenerateParticle()
{
	// 寿命を迎えていなければ && 終了命令が来ていなければ
	if (emitter_desc_.life_ > 0 && !emitter_desc_.is_dead_)
	{
		// 1フレーム中に指定された数を
		for (UINT count = emitter_desc_.gene_num_; count > 0; count--)
		{
			// 生成する
			Add(GenerateValue(emitter_desc_));
		}
	}
}

void Emitter::UpdateParticle()
{
	// マネージャ受け渡し用コンテナ
	forward_list<NcmParticleCommonArgs> temp;

	// 寿命を使用する設定なら
	if (emitter_desc_.use_life_ && emitter_desc_.life_ > 0)
	{
		// 寿命を削る
		emitter_desc_.life_--;
	}

	// 全てのパーティクルに対して
	for (auto &i : particles_)
	{
		// 更新
		i.Update();

		// マネージャ受け渡し用コンテナへ値を渡す
		temp.emplace_front();
		temp.front().pos = i.GetPos();
		temp.front().scale = i.GetScale();
		temp.front().color = i.GetColor();
		temp.front().alpha = i.GetAlpha();
	}

	// マネージャへ値を渡す
	NcmParticleManager::AddParticleCommonArgs(temp);

	// 寿命が尽きた粒をコンテナから削除
	particles_.remove_if([](Particle &x) { return x.GetIsDead(); });
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
	//PreDraw::SetPipeline(PipelineName::PlatePoly);
	// 全てのパーティクルに対して
	for (auto &i : shared_particles_)
	{
		// 使われているなら
		if (i.GetIsUsed())
		{
			// 更新する
			i.Draw();
		}
		// 使われていないなら
		else
		{
			// ループを抜ける
			break;
		}
	}
}

void Emitter::DebugDraw()
{
	NcmImGui::DragFloat3("pos", emitter_desc_.part_desc_.position_, 1.0f, -100.0f, 100.0f);
	NcmImGui::SliderFloat3("range", emitter_desc_.pos_rand_, 0.0f, 100.0f);
	NcmImGui::DragFloat3("velocity", emitter_desc_.part_desc_.velocity_, 0.1f, -5.0f, 5.0f);
	NcmImGui::SliderFloat3("vel_range", emitter_desc_.vel_rand_, 0.0f, 1.0f);
	ImGui::DragFloat("scale", &emitter_desc_.part_desc_.s_scale_, 0.1f, 0.1f, 50.0f);
	NcmImGui::SliderUINT("num", emitter_desc_.gene_num_, 0, 50);
	ImGui::Text("life : %d", emitter_desc_.life_);
	ImGui::Text("alpha : %d", emitter_desc_.part_desc_.alpha_);
	ImGui::Spacing();
	ImGui::Spacing();
}

void Emitter::StaticDebugDraw()
{
	//ImGui::Text("particle_num = %d", particle_num_);
}
