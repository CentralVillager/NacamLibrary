#include "Emitter.h"
#include "Utility.h"
#include "ImGuiManager.h"
#include <cmath>

using namespace DirectX;

std::unique_ptr<Model> Emitter::model_ = nullptr;

ParticleMember Emitter::GenerateValue(EmitterArgs emitter) {

	using namespace NacamUtility;

	{
		// �}�C�i�X�h�~
		emitter.pos_rand_.x = fabs(emitter.pos_rand_.x);
		emitter.pos_rand_.y = fabs(emitter.pos_rand_.y);
		emitter.pos_rand_.z = fabs(emitter.pos_rand_.z);

		// �ʒu�̃����_���l���擾
		XMFLOAT3 pos_rand = GenerateRandom((emitter.pos_rand_ / 2) - emitter.pos_rand_, emitter.pos_rand_ / 2);

		// �����_���l���ʒu�ɉ��Z
		emitter.particle.position_ = emitter.particle.position_ + pos_rand;
	}

	{
		// �}�C�i�X�h�~
		emitter.vel_rand_.x = fabs(emitter.vel_rand_.x);
		emitter.vel_rand_.y = fabs(emitter.vel_rand_.y);
		emitter.vel_rand_.z = fabs(emitter.vel_rand_.z);

		// ���x�̃����_���l���擾
		XMFLOAT3 vel_rand = GenerateRandom((emitter.vel_rand_ / 2) - emitter.vel_rand_, emitter.vel_rand_ / 2);

		// �����_���l�𑬓x�ɉ��Z
		emitter.particle.velocity_ = emitter.particle.velocity_ + vel_rand;
	}

	XMFLOAT3 acc{};
	acc.y = 0.001f;

	// �ݒ肳�ꂽ�l�ŗv�f���쐬
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

	// �擪�ɗv�f���\�z
	particles_.emplace_front();

	// �擪�̎Q�Ƃ��擾
	Particle &p_ = particles_.front();

	// ������
	p_.Initialize(model_.get(), p);
}

void Emitter::StaticInitialize() {

	// ���f���f�[�^�̓ǂݍ��݂Ɛ���
	model_ = make_unique<Model>();
	model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");
}

void Emitter::GenerateParticle() {

	// �������g�p����ݒ�Ȃ�
	if (emitter_args_.use_life_ && emitter_args_.life_ > 0) {

		// ���������
		emitter_args_.life_--;
	}

	// �������}���Ă��Ȃ���� && �I�����߂����Ă��Ȃ����
	if (emitter_args_.life_ > 0 && !emitter_args_.is_dead_) {

		// 1�t���[�����Ɏw�肳�ꂽ���𐶐�����
		for (UINT count = emitter_args_.gene_num_; count > 0; count--) {

			Add(GenerateValue(emitter_args_));
		}
	}

	// �������s���������R���e�i����폜
	particles_.remove_if([](Particle &x) { return x.GetIsDead(); });

	// �S�Ẵp�[�e�B�N�����X�V
	for (auto &i : particles_) {

		i.Update();
	}
}

void Emitter::PrepareTerminate() {

	// �I���t���O�������Ă��Ȃ�������
	if (!emitter_args_.is_dead_) {

		// �I���t���O�𗧂�����
		emitter_args_.is_dead_ = true;
	}
}

bool Emitter::NoticeCanTerminate() {

	// �p�[�e�B�N���R���e�i����ɂȂ�����
	if (particles_.empty()) {

		// �I���������o�����ƒm�点��
		return true;
	}

	return false;
}

void Emitter::Draw() {

	// �S�Ẵp�[�e�B�N����`��
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
