#include "UltimateManager.h"
#include "../../DebugUtill/NcmImGui.h"
#include "../../Utility/NcmUtil.h"
#include "../../../Lib/Win32App/Win32App.h"

using namespace NcmUtill;

std::array<ncm_thandle, UltimateManager::MAX_ULT_TEX_NUM_> UltimateManager::ult_tex_hub_;
ncm_thandle UltimateManager::percent_;
ncm_thandle UltimateManager::icon_q_;

UltimateManager::UltimateManager() :
	currect_ult_per_(0),
	max_ult_per_(100),
	value_dist_(0),
	is_triggering_(false),
	num_alpha_(0.0f),
	ease_(),
	num_alpha_to_max_(),
	num_alpha_to_min_(),
	to_max_(),
	to_min_(),
	is_change_(false),
	nums_(std::make_unique<Numbers>()),
	ult_tex_draw_intervel_(8),
	is_finish_display_(),
	ImGui_num_scale_(0.4f),
	ImGui_offset_x_(),
	ImGui_offset_all_y_(70.0f),
	ImGui_tex_scale_(2.0f),
	ImGui_tex_alpha_(0.2f)
{}

UltimateManager::~UltimateManager()
{}

void UltimateManager::LoadResources()
{
	using enum UltTex;

	ult_tex_hub_[(int)(u)] = NcmSprite::LoadTex("Resources/Textures/ultimate/u.png");
	ult_tex_hub_[(int)(l)] = NcmSprite::LoadTex("Resources/Textures/ultimate/l.png");
	ult_tex_hub_[(int)(t)] = NcmSprite::LoadTex("Resources/Textures/ultimate/t.png");
	ult_tex_hub_[(int)(i)] = NcmSprite::LoadTex("Resources/Textures/ultimate/i.png");
	ult_tex_hub_[(int)(m)] = NcmSprite::LoadTex("Resources/Textures/ultimate/m.png");
	ult_tex_hub_[(int)(a)] = NcmSprite::LoadTex("Resources/Textures/ultimate/a.png");
	ult_tex_hub_[(int)(T)] = NcmSprite::LoadTex("Resources/Textures/ultimate/t.png");
	ult_tex_hub_[(int)(e)] = NcmSprite::LoadTex("Resources/Textures/ultimate/e.png");
	ult_tex_hub_[(int)(stanby)] = NcmSprite::LoadTex("Resources/Textures/ultimate/stanby.png");

	percent_ = NcmSprite::LoadTex("Resources/Textures/numbers/%.png");
	NcmSprite::SetScale(percent_, 0.25f);
	NcmSprite::SetAnchorPoint(percent_, { 0, 1.0f });

	icon_q_ = NcmSprite::LoadTex("Resources/Textures/ultimate/icon_q.png");
	NcmSprite::SetScale(icon_q_, 0.1f);
	NcmSprite::SetAnchorPoint(icon_q_, { 0.5f, 0.5f });
	XMFLOAT2 pos = { Win32App::FCENTER_.x, Win32App::FCENTER_.y + 240.0f };
	NcmSprite::SetPos(icon_q_, pos);
}

void UltimateManager::Initialize()
{
	// �ݒ�\����
	NcmEaseDesc desc{};
	desc.ease_type = NcmEaseType::OutQuad;
	desc.init_value = 0;
	desc.total_move = (float)(value_dist_);
	desc.t_rate = 0.03f;
	ease_ = NcmEasing::RegisterEaseData(desc);

	desc.ease_type = NcmEaseType::OutQuad;
	desc.init_value = MIN_ALPHA_VALUE_;
	desc.total_move = MAX_ALPHA_VALUE_ - MIN_ALPHA_VALUE_;
	desc.t_rate = 0.03f;
	num_alpha_to_max_ = NcmEasing::RegisterEaseData(desc);

	desc.ease_type = NcmEaseType::OutQuad;
	desc.init_value = MAX_ALPHA_VALUE_;
	desc.total_move = MIN_ALPHA_VALUE_ - MAX_ALPHA_VALUE_;
	desc.t_rate = 0.03f;
	num_alpha_to_min_ = NcmEasing::RegisterEaseData(desc);
}

void UltimateManager::Update()
{
	static bool is_continuing = false;

	// �l���X�V���ꂽ��
	if (is_change_)
	{
		// �����������l�Ƃ���
		NcmEasing::SetInitValue(ease_, (float)(currect_ult_per_));
		uint32_t total_move = value_dist_ - currect_ult_per_;
		NcmEasing::SetTotalMove(ease_, (float)(total_move));

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
		currect_ult_per_ = (uint32_t)(NcmEasing::GetValue(ease_));

		// �ڕW�l�܂őJ�ڂ�����
		if (currect_ult_per_ >= value_dist_)
		{
			// �J�ڒ��t���O���~�낷
			is_continuing = false;
			// ���Ԃ����Z�b�g����
			NcmEasing::ResetTime(ease_);
		}
	}

	/* �ȉ�%���J�ڏ��� */

	if (currect_ult_per_ < max_ult_per_)
	{
		num_alpha_ = MAX_ALPHA_VALUE_;
		return;
	}
	// �p�[�Z���g���ő�܂ŗ��܂�����

	// ���l���ő�܂œ��B������
	if (num_alpha_ >= MAX_ALPHA_VALUE_)
	{
		to_max_ = false;
		to_min_ = true;

		NcmEasing::ResetTime(num_alpha_to_max_);
	}

	// ���l���ŏ��܂œ��B������
	if (num_alpha_ <= MIN_ALPHA_VALUE_)
	{
		to_max_ = true;
		to_min_ = false;

		NcmEasing::ResetTime(num_alpha_to_min_);
	}

	// ���l���ő�܂őJ�ڒ�
	if (to_max_)
	{
		// �C�[�W���O�̒l���X�V
		NcmEasing::UpdateValue(num_alpha_to_max_);

		// ���̒l���擾
		num_alpha_ = NcmEasing::GetValue(num_alpha_to_max_);
	}

	// ���l���ŏ��܂őJ�ڒ�
	if (to_min_)
	{
		// �C�[�W���O�̒l���X�V
		NcmEasing::UpdateValue(num_alpha_to_min_);

		// ���̒l���擾
		num_alpha_ = NcmEasing::GetValue(num_alpha_to_min_);
	}
}

void UltimateManager::Draw()
{}

void UltimateManager::DrawUi()
{
	using enum HorizontalAlignment;
	using enum VerticalAlignment;

	// ULT�p�[�Z���e�[�W�̕`��
	nums_->DrawNumber(currect_ult_per_, ImGui_num_scale_, num_alpha_, Center, Bottom);

	// Q�A�C�R���̕`��
	if (currect_ult_per_ >= max_ult_per_)
	{
		NcmSprite::SetAlpha(icon_q_, num_alpha_);
		NcmSprite::DrawTex(icon_q_);
	}

	// %�e�N�X�`���̕`��
	NcmSprite::SetAlpha(percent_, num_alpha_);
	NcmSprite::DrawTex(percent_, { Win32App::FCENTER_.x + (nums_->GetDigitsWidth() / 2), Win32App::FSIZE_.y - nums_->GetDeadLineSize() });

	// �t���ɂȂ�����
	if (NoticeFullCharged())
	{
		// �\�����I������܂�
		if (!is_finish_display_)
		{
			// UI���o��`��
			DrawUltimateStanby();
		}
	}
}

void UltimateManager::DrawUltimateStanby()
{
	// �`��t�F�[�Y
	static bool is_draw_phase_ult = true;

	// �t�F�[�Y�� "ULTIMATE" �Ȃ�
	if (is_draw_phase_ult)
	{
		// �S�ĕ`�悵��������
		if (DrawULTIMATESequence())
		{
			// �t�F�[�Y��؂�ւ���
			is_draw_phase_ult = false;
		}
	}

	// �t�F�[�Y�� "STANBY" �Ȃ�
	if (!is_draw_phase_ult)
	{
		// �\���̏I�����L�^����
		is_finish_display_ = DrawSTANBYSequence();
	}

	// �\�����I�������
	if (is_finish_display_)
	{
		// �t�F�[�Y�����Z�b�g����
		is_draw_phase_ult = true;
	}
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
	ImGui::Text("TEX");
	ImGui::DragFloat("num_scale", &ImGui_num_scale_, 0.1f, 0.1f, 10.0f);
	ImGui::DragFloat("tex_scale", &ImGui_tex_scale_, 0.1f, 0.1f, 10.0f);
	ImGui::DragFloat("tex_alpha", &ImGui_tex_alpha_, 0.1f, 0.1f, 1.0f);
	ImGui::DragFloat("x_offset", &ImGui_offset_x_, 0.1f, -10.0f, 100.0f);
	ImGui::DragFloat("all_y_offset", &ImGui_offset_all_y_, 0.1f, -10.0f, 100.0f);
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
	is_finish_display_ = false;
}

bool UltimateManager::DrawULTIMATESequence()
{
	// ���ő�\����
	static int32_t max_view_num = 0;
	static int32_t viewer_count = ult_tex_draw_intervel_;

	static bool is_first = true;

	if (IsZeroOrLess(viewer_count))
	{
		// stanby�������S�Ẵe�N�X�`�����ɒB���Ă��Ȃ�������
		if (max_view_num < ult_tex_hub_.size())
		{
			// �ő�\�����𑝂₷
			max_view_num++;
		}

		// �J�E���g�����Z�b�g
		viewer_count = ult_tex_draw_intervel_;
	}

	// stanby�̕\�����ԂɂȂ�����
	if (max_view_num > (int)(UltTex::stanby))
	{
		// ���Z�b�g
		max_view_num = 0;
		is_first = true;
		return true;
	}

	viewer_count--;

	// �e�N�X�`���T�C�Y�ۑ��p
	XMFLOAT2 size_stack{};

	for (int i = (int)(UltTex::u); i <= max_view_num - 1; i++)
	{
		// �F��ݒ�
		XMFLOAT4 color = NcmSprite::GetColor(ult_tex_hub_[i]);
		color.w = ImGui_tex_alpha_;
		NcmSprite::SetColor(ult_tex_hub_[i], color);

		if (i <= (int)(UltTex::i))
		{
			is_first = true;
		}
		else if (i >= (int)(UltTex::m) && is_first)
		{
			// ����X�V
			size_stack.x = 0;
			size_stack.y = NcmSprite::GetSize(ult_tex_hub_[(int)(UltTex::u)]).y;
			is_first = false;
		}

		// �X�P�[�����Z�b�g
		NcmSprite::SetScale(ult_tex_hub_[i], ImGui_tex_scale_);
		// �e�N�X�`����`��
		NcmSprite::DrawTex(ult_tex_hub_[i], { size_stack.x + ImGui_offset_x_ * i, size_stack.y + ImGui_offset_all_y_ });
		// �T�C�Y���L��
		size_stack.x += NcmSprite::GetSize(ult_tex_hub_[i]).x;
	}

	return false;
}

bool UltimateManager::DrawSTANBYSequence()
{
	// �\���񐔐ݒ�
	static const int DO_DISPLAY_COUNT = 40;
	// �\����
	static int display_count = 0;

	// �\���^�C�~���O���v�Z
	if (CheckDoDisplay())
	{
		// �A���J�[�|�C���g��ݒ�
		NcmSprite::SetAnchorPoint(ult_tex_hub_[(int)(UltTex::stanby)], { 0.5f, 0.5f });

		// �F��ݒ�
		XMFLOAT4 color = NcmSprite::GetColor(ult_tex_hub_[(int)(UltTex::stanby)]);
		color.w = ImGui_tex_alpha_;
		NcmSprite::SetColor(ult_tex_hub_[(int)(UltTex::stanby)], color);

		// �`��
		NcmSprite::DrawTex(ult_tex_hub_[(int)(UltTex::stanby)], Win32App::FCENTER_);

		// �\���񐔂��J�E���g
		display_count++;

		// �����\��������
		if (display_count >= DO_DISPLAY_COUNT)
		{
			display_count = 0;
			return true;
		}
	}

	return false;
}

bool UltimateManager::CheckDoDisplay()
{
	int visible_time = 5;
	int invisivle_time = 5;
	static int visi_count = visible_time;
	static int invi_count = invisivle_time;

	if (visi_count >= 0)
	{
		visi_count--;
		return true;
	}
	else
	{
		invi_count--;

		if (invi_count <= 0)
		{
			visi_count = visible_time;
			invi_count = invisivle_time;
		}

		return false;
	}
}
