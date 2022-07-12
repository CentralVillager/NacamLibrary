#include "Emitter.h"
#include "Utility.h"
#include "ImGuiManager.h"
#include <cmath>

using namespace DirectX;

std::unique_ptr<Model> Emitter::model_ = nullptr;

ParticleMember Emitter::GenerateValue(EmitterArgs emitter) {

	using namespace NacamUtility;

	{
		// マイナス防止
		emitter.pos_rand_.x = fabs(emitter.pos_rand_.x);
		emitter.pos_rand_.y = fabs(emitter.pos_rand_.y);
		emitter.pos_rand_.z = fabs(emitter.pos_rand_.z);

		// 位置のランダム値を取得
		XMFLOAT3 pos_rand = GenerateRandom((emitter.pos_rand_ / 2) - emitter.pos_rand_, emitter.pos_rand_ / 2);

		// ランダム値を位置に加算
		emitter.particle.position_ = emitter.particle.position_ + pos_rand;
	}

	{
		// マイナス防止
		emitter.vel_rand_.x = fabs(emitter.vel_rand_.x);
		emitter.vel_rand_.y = fabs(emitter.vel_rand_.y);
		emitter.vel_rand_.z = fabs(emitter.vel_rand_.z);

		// 速度のランダム値を取得
		XMFLOAT3 vel_rand = GenerateRandom((emitter.vel_rand_ / 2) - emitter.vel_rand_, emitter.vel_rand_ / 2);

		// ランダム値を速度に加算
		emitter.particle.velocity_ = emitter.particle.velocity_ + vel_rand;
	}

	XMFLOAT3 acc{};
	acc.y = 0.001f;

	// 設定された値で要素を作成
	ParticleMember _p{};
	_p.position_ = emitter.particle.position_;
	_p.velocity_ = emitter.particle.velocity_;
	_p.accel_ = acc;
	_p.scale_ = _p.s_scale_ = emitter.particle.s_scale_;
	_p.e_scale_ = 0.0f;
	_p.life_ = emitter.particle.life_;

	return _p;
}

void Emitter::Add(ParticleMember p) {

	// 先頭に要素を構築
	particles_.emplace_front();

	// 先頭の参照を取得
	Particle &p_ = particles_.front();

	// 初期化
	p_.Initialize(model_.get(), p);
}

void Emitter::StaticInitialize() {

	// モデルデータの読み込みと生成
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
}

void Emitter::GenerateParticle() {

	// 寿命を使用する設定なら
	if (emitter_args_.use_life_ && emitter_args_.life_ > 0) {

		// 寿命を削る
		emitter_args_.life_--;
	}

	// 寿命を迎えていなければ && 終了命令が来ていなければ
	if (emitter_args_.life_ > 0 && !emitter_args_.is_dead_) {

		// 1フレーム中に指定された数を生成する
		for (UINT count = emitter_args_.gene_num_; count > 0; count--) {

			Add(GenerateValue(emitter_args_));
		}
	}

	// 寿命が尽きた粒をコンテナから削除
	particles_.remove_if([](Particle &x) { return x.GetIsDead(); });

	// 全てのパーティクルを更新
	for (auto &i : particles_) {

		i.Update();
	}
}

void Emitter::PrepareTerminate() {

	// 終了フラグが立っていなかったら
	if (!emitter_args_.is_dead_) {

		// 終了フラグを立たせる
		emitter_args_.is_dead_ = true;
	}
}

bool Emitter::NoticeCanTerminate() {

	// パーティクルコンテナが空になったら
	if (particles_.empty()) {

		// 終了準備が出来たと知らせる
		return true;
	}

	return false;
}

void Emitter::Draw() {

	// 全てのパーティクルを描画
	for (auto &i : particles_) {

		i.Draw();
	}
}

void Emitter::DebugDraw() {

	ImGuiManager::DragFloat3("pos", emitter_args_.particle.position_, 1.0f, -100.0f, 100.0f);
	ImGuiManager::SliderFloat3("range", emitter_args_.pos_rand_, 0.0f, 100.0f);
	ImGuiManager::DragFloat3("velocity", emitter_args_.particle.velocity_, 0.1f, -5.0f, 5.0f);
	ImGuiManager::SliderFloat3("vel_range", emitter_args_.vel_rand_, 0.0f, 1.0f);
	ImGui::DragFloat("scale", &emitter_args_.particle.s_scale_, 0.1f, 0.1f, 50.0f);
	ImGui::DragInt("life", &emitter_args_.particle.life_, 1.0f, 0, 1000);
	ImGuiManager::SliderUINT("num", emitter_args_.gene_num_, 0, 50);
	ImGui::Text("life : %d", emitter_args_.life_);
	ImGui::Spacing();
	ImGui::Spacing();
}
