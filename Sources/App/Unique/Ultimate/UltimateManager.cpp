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
	// 設定構造体
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

	// 値が更新されたら
	if (is_change_)
	{
		// 現％を初期値とする
		NcmEasing::SetInitValue(ease_, (float)(currect_ult_per_));
		uint32_t total_move = value_dist_ - currect_ult_per_;
		NcmEasing::SetTotalMove(ease_, (float)(total_move));

		// 遷移中フラグを立てる
		is_continuing = true;

		// 更新フラグを降ろす
		is_change_ = false;
	}

	// 値が遷移中なら
	if (is_continuing)
	{
		// 遷移の値を更新する
		NcmEasing::UpdateValue(ease_);

		// その値を代入する
		currect_ult_per_ = (uint32_t)(NcmEasing::GetValue(ease_));

		// 目標値まで遷移したら
		if (currect_ult_per_ >= value_dist_)
		{
			// 遷移中フラグを降ろす
			is_continuing = false;
			// 時間をリセットする
			NcmEasing::ResetTime(ease_);
		}
	}

	/* 以下%α遷移処理 */

	if (currect_ult_per_ < max_ult_per_)
	{
		num_alpha_ = MAX_ALPHA_VALUE_;
		return;
	}
	// パーセントが最大まで溜まったら

	// α値が最大まで到達したら
	if (num_alpha_ >= MAX_ALPHA_VALUE_)
	{
		to_max_ = false;
		to_min_ = true;

		NcmEasing::ResetTime(num_alpha_to_max_);
	}

	// α値が最小まで到達したら
	if (num_alpha_ <= MIN_ALPHA_VALUE_)
	{
		to_max_ = true;
		to_min_ = false;

		NcmEasing::ResetTime(num_alpha_to_min_);
	}

	// α値を最大まで遷移中
	if (to_max_)
	{
		// イージングの値を更新
		NcmEasing::UpdateValue(num_alpha_to_max_);

		// その値を取得
		num_alpha_ = NcmEasing::GetValue(num_alpha_to_max_);
	}

	// α値を最小まで遷移中
	if (to_min_)
	{
		// イージングの値を更新
		NcmEasing::UpdateValue(num_alpha_to_min_);

		// その値を取得
		num_alpha_ = NcmEasing::GetValue(num_alpha_to_min_);
	}
}

void UltimateManager::Draw()
{}

void UltimateManager::DrawUi()
{
	using enum HorizontalAlignment;
	using enum VerticalAlignment;

	// ULTパーセンテージの描画
	nums_->DrawNumber(currect_ult_per_, ImGui_num_scale_, num_alpha_, Center, Bottom);

	// Qアイコンの描画
	if (currect_ult_per_ >= max_ult_per_)
	{
		NcmSprite::SetAlpha(icon_q_, num_alpha_);
		NcmSprite::DrawTex(icon_q_);
	}

	// %テクスチャの描画
	NcmSprite::SetAlpha(percent_, num_alpha_);
	NcmSprite::DrawTex(percent_, { Win32App::FCENTER_.x + (nums_->GetDigitsWidth() / 2), Win32App::FSIZE_.y - nums_->GetDeadLineSize() });

	// フルになったら
	if (NoticeFullCharged())
	{
		// 表示が終了するまで
		if (!is_finish_display_)
		{
			// UI演出を描画
			DrawUltimateStanby();
		}
	}
}

void UltimateManager::DrawUltimateStanby()
{
	// 描画フェーズ
	static bool is_draw_phase_ult = true;

	// フェーズが "ULTIMATE" なら
	if (is_draw_phase_ult)
	{
		// 全て描画しきったら
		if (DrawULTIMATESequence())
		{
			// フェーズを切り替える
			is_draw_phase_ult = false;
		}
	}

	// フェーズが "STANBY" なら
	if (!is_draw_phase_ult)
	{
		// 表示の終了を記録する
		is_finish_display_ = DrawSTANBYSequence();
	}

	// 表示が終わったら
	if (is_finish_display_)
	{
		// フェーズをリセットする
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
	// 上限に達していなかったら
	if (currect_ult_per_ < max_ult_per_)
	{
		// 値を加算する
		value_dist_ += v;

		// 加算した結果、上限を超えたら
		if (value_dist_ > max_ult_per_)
		{
			// 遷移目標を100にする
			value_dist_ = 100;
		}

		// 値更新フラグを立てる
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
	// 現最大表示数
	static int32_t max_view_num = 0;
	static int32_t viewer_count = ult_tex_draw_intervel_;

	static bool is_first = true;

	if (IsZeroOrLess(viewer_count))
	{
		// stanbyを除く全てのテクスチャ数に達していなかったら
		if (max_view_num < ult_tex_hub_.size())
		{
			// 最大表示数を増やす
			max_view_num++;
		}

		// カウントをリセット
		viewer_count = ult_tex_draw_intervel_;
	}

	// stanbyの表示時間になったら
	if (max_view_num > (int)(UltTex::stanby))
	{
		// リセット
		max_view_num = 0;
		is_first = true;
		return true;
	}

	viewer_count--;

	// テクスチャサイズ保存用
	XMFLOAT2 size_stack{};

	for (int i = (int)(UltTex::u); i <= max_view_num - 1; i++)
	{
		// 色を設定
		XMFLOAT4 color = NcmSprite::GetColor(ult_tex_hub_[i]);
		color.w = ImGui_tex_alpha_;
		NcmSprite::SetColor(ult_tex_hub_[i], color);

		if (i <= (int)(UltTex::i))
		{
			is_first = true;
		}
		else if (i >= (int)(UltTex::m) && is_first)
		{
			// 基準を更新
			size_stack.x = 0;
			size_stack.y = NcmSprite::GetSize(ult_tex_hub_[(int)(UltTex::u)]).y;
			is_first = false;
		}

		// スケールをセット
		NcmSprite::SetScale(ult_tex_hub_[i], ImGui_tex_scale_);
		// テクスチャを描画
		NcmSprite::DrawTex(ult_tex_hub_[i], { size_stack.x + ImGui_offset_x_ * i, size_stack.y + ImGui_offset_all_y_ });
		// サイズを記憶
		size_stack.x += NcmSprite::GetSize(ult_tex_hub_[i]).x;
	}

	return false;
}

bool UltimateManager::DrawSTANBYSequence()
{
	// 表示回数設定
	static const int DO_DISPLAY_COUNT = 40;
	// 表示回数
	static int display_count = 0;

	// 表示タイミングを計算
	if (CheckDoDisplay())
	{
		// アンカーポイントを設定
		NcmSprite::SetAnchorPoint(ult_tex_hub_[(int)(UltTex::stanby)], { 0.5f, 0.5f });

		// 色を設定
		XMFLOAT4 color = NcmSprite::GetColor(ult_tex_hub_[(int)(UltTex::stanby)]);
		color.w = ImGui_tex_alpha_;
		NcmSprite::SetColor(ult_tex_hub_[(int)(UltTex::stanby)], color);

		// 描画
		NcmSprite::DrawTex(ult_tex_hub_[(int)(UltTex::stanby)], Win32App::FCENTER_);

		// 表示回数をカウント
		display_count++;

		// 既定回表示したら
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
