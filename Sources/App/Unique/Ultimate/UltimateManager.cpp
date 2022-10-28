#include "UltimateManager.h"
#include "../../Debug/NcmImGui.h"

std::array<int, UltimateManager::MAX_ULT_TEX_NUM_> UltimateManager::tex_hub_;

UltimateManager::UltimateManager() :
	currect_ult_per_(100),
	max_ult_per_(100),
	is_triggering_(false)
{}

UltimateManager::~UltimateManager()
{}

void UltimateManager::LoadResources()
{
	using enum UltTex;
	tex_hub_[(int)(u)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/u.png");
	tex_hub_[(int)(l)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/l.png");
	tex_hub_[(int)(t)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/t.png");
	tex_hub_[(int)(i)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/i.png");
	tex_hub_[(int)(m)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/m.png");
	tex_hub_[(int)(a)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/a.png");
	tex_hub_[(int)(T)] = tex_hub_[(int)(t)];
	tex_hub_[(int)(e)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/e.png");
	tex_hub_[(int)(stanby)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/stanby.png");
}

void UltimateManager::Initialize()
{}

void UltimateManager::Update()
{}

void UltimateManager::Draw()
{}

void UltimateManager::DrawUi()
{
	using enum UltTex;

	float size_stack = NcmSprite::GetSize(tex_hub_[(int)(u)]).x;

	NcmSprite::DrawTex(tex_hub_[(int)(u)]);
}

void UltimateManager::DebugDraw()
{
	if (ImGui::Button("Add Ult"))
	{
		AddUltValue(10);
	}

	if (NoticeFullCharged())
	{
		if (ImGui::Button("Triggering Ult"))
		{
			TriggeringUlt();
		}
	}

	NcmImGui::DragInt("ULT", currect_ult_per_, 1.0f, 0, 100);
	NcmImGui::DragInt("MAX_ULT", max_ult_per_, 1.0f, 0, 100);
}

void UltimateManager::AddUltValue(uint32_t v)
{
	if (currect_ult_per_ < max_ult_per_)
	{
		currect_ult_per_ += v;
	}
}

bool UltimateManager::NoticeFullCharged()
{
	return currect_ult_per_ >= max_ult_per_;
}

void UltimateManager::TriggeringUlt()
{
	currect_ult_per_ = 100;
}
