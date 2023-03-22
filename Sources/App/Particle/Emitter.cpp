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
		// �}�C�i�X�h�~
		emi.pos_rand_.x = fabs(emi.pos_rand_.x);
		emi.pos_rand_.y = fabs(emi.pos_rand_.y);
		emi.pos_rand_.z = fabs(emi.pos_rand_.z);

		// �ʒu�̃����_���l���擾
		XMFLOAT3 pos_rand = GenerateRandom((emi.pos_rand_ / 2) - emi.pos_rand_, emi.pos_rand_ / 2);

		// �����_���l���ʒu�ɉ��Z
		emi.part_desc_.position_ = emi.part_desc_.position_ + pos_rand;
	}

	{
		// �}�C�i�X�h�~
		emi.vel_rand_.x = fabs(emi.vel_rand_.x);
		emi.vel_rand_.y = fabs(emi.vel_rand_.y);
		emi.vel_rand_.z = fabs(emi.vel_rand_.z);

		// ���x�̃����_���l���擾
		XMFLOAT3 vel_rand = GenerateRandom((emi.vel_rand_ / 2) - emi.vel_rand_, emi.vel_rand_ / 2);

		// �����_���l�𑬓x�ɉ��Z
		emi.part_desc_.velocity_ = emi.part_desc_.velocity_ + vel_rand;
	}

	XMFLOAT3 acc{};
	acc.y = 0.000f;

	// �ݒ肳�ꂽ�l�ŗv�f���쐬
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
	// �擪�ɗv�f���\�z
	particles_.emplace_front();
	// ������
	particles_.front().Initialize(model_.get(), p);
}

void Emitter::LoadResources()
{
	if (!model_)
	{
		// ���f���f�[�^�̓ǂݍ��݂Ɛ���
		model_ = make_unique<Model>();
		model_->LoadObjModel("Resources/Ball/", "smooth_ball.obj", "smooth_ball.mtl");

		tex_handle_ = NcmSprite::LoadTex("Resources/textures/particle/effect1.png");
	}
}

void Emitter::Update()
{
	// �������g�p����ݒ�Ȃ�
	if (emitter_desc_.use_life_ && emitter_desc_.life_ > 0)
	{
		// ���������
		emitter_desc_.life_--;
	}

	// �������}���Ă��Ȃ���� && �I�����߂����Ă��Ȃ����
	if (emitter_desc_.life_ > 0 && !emitter_desc_.is_dead_)
	{
		// �Ō���̎��̎Q�Ƃ��擾
		auto itr = shared_particles_.end();

		// 1�t���[�����Ɏw�肳�ꂽ�����g��
		for (UINT count = emitter_desc_.gene_num_; count > 0; count--)
		{
			UseParticle(itr);
			updated_dirty_ = true;
		}
	}

	// �p�[�e�B�N������������Ă�����
	if (updated_dirty_)
	{
		// is_used_ �t���O�������Ă���v�f��O���փ\�[�g
		shared_particles_.sort([](Particle &lhs, Particle &rhs) { return lhs.GetIsUsed() > rhs.GetIsUsed(); });
		updated_dirty_ = false;
	}

	// �S�Ẵp�[�e�B�N���ɑ΂���
	for (auto &i : shared_particles_)
	{
		// �g���Ă���Ȃ�
		if (i.GetIsUsed())
		{
			// �X�V����
			i.Update();
		}
		// �g���Ă��Ȃ��Ȃ�
		else
		{
			// ���[�v�𔲂���
			break;
		}
		// ����ł�����
		if (i.GetIsDead())
		{
			// ����������
			i.SetIsUsed(false);
			// �l�����Z�b�g����
			i.ResetParamater();
			particle_num_--;
		}
	}

	// is_used_ �t���O�������Ă���v�f��O���փ\�[�g
	shared_particles_.sort([](Particle &lhs, Particle &rhs) { return lhs.GetIsUsed() > rhs.GetIsUsed(); });
}

void Emitter::UseParticle(std::list<Particle>::iterator last_itr)
{
	// �Ō�����珇�Ɏg�p
	last_itr--;

	// ���̗v�f��L��������
	last_itr->SetIsUsed(true);

	// �p�����[�^��ݒ�
	last_itr->SetParticleValue(GenerateValue(emitter_desc_));

	particle_num_++;
}

void Emitter::GenerateParticle()
{
	// �������}���Ă��Ȃ���� && �I�����߂����Ă��Ȃ����
	if (emitter_desc_.life_ > 0 && !emitter_desc_.is_dead_)
	{
		// 1�t���[�����Ɏw�肳�ꂽ����
		for (UINT count = emitter_desc_.gene_num_; count > 0; count--)
		{
			// ��������
			Add(GenerateValue(emitter_desc_));
		}
	}
}

void Emitter::UpdateParticle()
{
	// �}�l�[�W���󂯓n���p�R���e�i
	forward_list<NcmParticleCommonArgs> temp;

	// �������g�p����ݒ�Ȃ�
	if (emitter_desc_.use_life_ && emitter_desc_.life_ > 0)
	{
		// ���������
		emitter_desc_.life_--;
	}

	// �S�Ẵp�[�e�B�N���ɑ΂���
	for (auto &i : particles_)
	{
		// �X�V
		i.Update();

		// �}�l�[�W���󂯓n���p�R���e�i�֒l��n��
		temp.emplace_front();
		temp.front().pos = i.GetPos();
		temp.front().scale = i.GetScale();
		temp.front().color = i.GetColor();
		temp.front().alpha = i.GetAlpha();
	}

	// �}�l�[�W���֒l��n��
	NcmParticleManager::AddParticleCommonArgs(temp);

	// �������s���������R���e�i����폜
	particles_.remove_if([](Particle &x) { return x.GetIsDead(); });
}

void Emitter::PrepareTerminate()
{
	// �I���t���O�������Ă��Ȃ�������
	if (!emitter_desc_.is_dead_)
	{
		// �I���t���O�𗧂�����
		emitter_desc_.is_dead_ = true;
	}
}

bool Emitter::NoticeCanTerminate()
{
	// �p�[�e�B�N���R���e�i����ɂȂ�����
	if (particles_.empty())
	{
		// �I���������o�����ƒm�点��
		return true;
	}

	return false;
}

void Emitter::Draw()
{
	//PreDraw::SetPipeline(PipelineName::PlatePoly);
	// �S�Ẵp�[�e�B�N���ɑ΂���
	for (auto &i : shared_particles_)
	{
		// �g���Ă���Ȃ�
		if (i.GetIsUsed())
		{
			// �X�V����
			i.Draw();
		}
		// �g���Ă��Ȃ��Ȃ�
		else
		{
			// ���[�v�𔲂���
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
