#include "UltimateManager.h"
#include "../../Debug/NcmImGui.h"

std::array<int, UltimateManager::MAX_ULT_TEX_NUM_> UltimateManager::ult_tex_hub_;
ncm_thandle UltimateManager::percent_;

UltimateManager::UltimateManager() :
	currect_ult_per_(0),
	max_ult_per_(100),
	value_dist_(0),
	is_triggering_(false),
	ease_(),
	is_change_(false),
	nums_(std::make_unique<Numbers>())
{}

UltimateManager::~UltimateManager()
{}

void UltimateManager::LoadResources()
{
	using enum UltTex;
	ult_tex_hub_[(int)(u)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/u.png");
	ult_tex_hub_[(int)(l)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/l.png");
	ult_tex_hub_[(int)(t)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/t.png");
	ult_tex_hub_[(int)(i)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/i.png");
	ult_tex_hub_[(int)(m)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/m.png");
	ult_tex_hub_[(int)(a)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/a.png");
	ult_tex_hub_[(int)(T)] = ult_tex_hub_[(int)(t)];
	ult_tex_hub_[(int)(e)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/e.png");
	ult_tex_hub_[(int)(stanby)] = NcmSprite::LoadTex(L"Resources/Textures/ultimate/stanby.png");

	percent_ = NcmSprite::LoadTex(L"Resources/Textures/numbers/%.png");
	NcmSprite::SetScale(percent_, 0.25f);
	NcmSprite::SetAnchorPoint(percent_, { 0, 1.0f });
}

void UltimateManager::Initialize()
{
	// �ݒ�\����
	NcmEaseDesc desc{};
	desc.ease_type = NcmEaseType::OutQuad;
	desc.init_value = 0;
	desc.total_move = value_dist_;
	desc.t_rate = 0.03f;
	ease_ = NcmEasing::RegisterEaseData(desc);
}

void UltimateManager::Update()
{
	static bool is_continuing = false;

	// �l���X�V���ꂽ��
	if (is_change_)
	{
		// �����������l�Ƃ���
		NcmEasing::SetInitValue(ease_, currect_ult_per_);
		uint32_t total_move = value_dist_ - currect_ult_per_;
		NcmEasing::SetTotalMove(ease_, total_move);

		// �J�ڒ��t���O�𗧂Ă�
		is_continuing = true;

		// �X�V�t���O���~�낷
		is_change_ = false;
	}

	// �l���J�ڒ��Ȃ�
	if (is_continuing)
	{
		// �J�ڂ̒l���X�V����
		NcmEasing::UpdateValue(ease_);

		// ���̒l��������
		currect_ult_per_ = NcmEasing::GetValue(ease_);

		// �ڕW�l�܂őJ�ڂ�����
		if (currect_ult_per_ >= value_dist_)
		{
			// �J�ڒ��t���O���~�낷
			is_continuing = false;
			// ���Ԃ����Z�b�g����
			NcmEasing::ResetTime(ease_);
		}
	}
}

void UltimateManager::Draw()
{}

void UltimateManager::DrawUi()
{
	using enum HorizontalAlignment;
	using enum VerticalAlignment;

	nums_->DrawNumber(currect_ult_per_, ImGui_num_scale_, Center, Bottom);
	NcmSprite::DrawTex(percent_, { Win32App::FCENTER_.x + (nums_->GetDigitsWidth() / 2), Win32App::FSIZE_.y - nums_->GetDeadLineSize() });
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
	ImGui::Text("dist : %d", value_dist_);
	ImGui::DragFloat("scale", &ImGui_num_scale_, 0.1f, 0.1f, 10.0f);
}

void UltimateManager::AddUltValue(uint32_t v)
{
	// ����ɒB���Ă��Ȃ�������
	if (currect_ult_per_ < max_ult_per_)
	{
		// �l�����Z����
		value_dist_ += v;

		// ���Z�������ʁA����𒴂�����
		if (value_dist_ > max_ult_per_)
		{
			// �J�ږڕW��100�ɂ���
			value_dist_ = 100;
		}

		// �l�X�V�t���O�𗧂Ă�
		is_change_ = true;
	}
}

bool UltimateManager::NoticeFullCharged()
{
	return currect_ult_per_ >= max_ult_per_;
}

void UltimateManager::TriggeringUlt()
{
	currect_ult_per_ = 0;
	is_change_ = true;
	value_dist_ = 0;
}
