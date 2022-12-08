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
	// 渡されたデータを併合する（merge()を使いたい）
	// 渡された全ての要素に対して
	for (auto i = args.begin(); i != args.end();)
	{
		part_args_.emplace_front();		// 新しく要素を構築
		auto itr = part_args_.begin();	// 参照を取得
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
	// 頂点情報を更新
	poly_->UpdateVertBuffer(part_args_);
	// 定数バッファを更新
	poly_->UpdateConstBuffer(part_args_);

	// 現在の総パーティクル数を計算
	particle_num = std::distance(part_args_.begin(), part_args_.end());

	// 最高保持数を更新
	if (particle_num >= max_particle_num)
	{
		max_particle_num = particle_num;
	}
}

void NcmParticleManager::Draw()
{
	// 描画
	poly_->Draw();
}

void NcmParticleManager::StaticDebugDraw()
{
	ImGui::Text("particle_num : %d", particle_num);
	ImGui::Text("max_particle_num : %d", max_particle_num);
}
