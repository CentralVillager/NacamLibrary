#include "NcmParticleManager.h"
#include "../../App/Debug/NcmImGui.h"

using namespace std;

std::forward_list<NcmParticleCommonArgs> NcmParticleManager::part_args_;
size_t NcmParticleManager::particle_num = 0;
size_t NcmParticleManager::max_particle_num = 0;

NcmParticleManager::NcmParticleManager() :
	poly_(make_unique<NcmPlatePoly>())
{}

NcmParticleManager::~NcmParticleManager()
{}

void NcmParticleManager::AddParticleCommonArgs(forward_list<NcmParticleCommonArgs> &args)
{
	// �n���ꂽ�f�[�^�𕹍�����imerge()���g�������j
	// �n���ꂽ�S�Ă̗v�f�ɑ΂���
	for (auto i = args.begin(); i != args.end();)
	{
		part_args_.emplace_front();		// �V�����v�f���\�z
		auto itr = part_args_.begin();	// �Q�Ƃ��擾
		itr->pos = i->pos;
		itr->scale = i->scale * 2;
		itr->alpha = i->alpha;

		i++;
	}
}

void NcmParticleManager::ClearParticleArgsBeforeUpdate()
{
	part_args_.clear();
}

void NcmParticleManager::Initialize()
{
	poly_->Initialize();
}

void NcmParticleManager::Update()
{
	// ���_�����X�V
	poly_->UpdateVertBuffer(part_args_);
	// �萔�o�b�t�@���X�V
	poly_->UpdateConstBuffer(part_args_);

	// ���݂̑��p�[�e�B�N�������v�Z
	particle_num = std::distance(part_args_.begin(), part_args_.end());

	// �ō��ێ������X�V
	if (particle_num >= max_particle_num)
	{
		max_particle_num = particle_num;
	}
}

void NcmParticleManager::Draw()
{
	// �`��
	poly_->Draw();
}

void NcmParticleManager::StaticDebugDraw()
{
	ImGui::Text("particle_num : %d", particle_num);
	ImGui::Text("max_particle_num : %d", max_particle_num);
}
