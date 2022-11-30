#include "NcmUi.h"
#include "../../Lib/Win32App/Win32App.h"
#include "../Utility/NcmUtil.h"

using namespace DirectX;
using namespace NcmUtill;

std::unique_ptr<Numbers> NcmUi::numbers_;
std::unordered_map<std::string, int> NcmUi::ui_hub_;
int32_t NcmUi::count_;
XMFLOAT2 NcmUi::bar_size_;
float NcmUi::bottom_pos_;
std::array<int, 3> NcmUi::hp_;
ncm_ehandle NcmUi::ease_;

NcmUi::NcmUi()
{}

NcmUi::~NcmUi()
{
	/*if (!ui_hub_.empty())
	{
		ui_hub_.clear();
	}*/
}

void NcmUi::Initialize()
{
	NcmEaseDesc desc{};
	desc.ease_type = NcmEaseType::OutCubic;
	desc.init_value = 0.0f;
	desc.total_move = 10.0f;
	desc.t_rate = 0.05f;
	ease_ = NcmEasing::RegisterEaseData(desc);
}

void NcmUi::LoadResources()
{
	Numbers::LoadResources();

	ui_hub_.emplace("slash", NcmSprite::LoadTex(L"Resources/Textures/SC/slash.png"));
	ui_hub_.emplace("0", NcmSprite::LoadTex(L"Resources/Textures/SC/0.png"));
	ui_hub_.emplace("1", NcmSprite::LoadTex(L"Resources/Textures/SC/1.png"));
	ui_hub_.emplace("2", NcmSprite::LoadTex(L"Resources/Textures/SC/2.png"));
	ui_hub_.emplace("3", NcmSprite::LoadTex(L"Resources/Textures/SC/3.png"));
	ui_hub_.emplace("4", NcmSprite::LoadTex(L"Resources/Textures/SC/4.png"));
	ui_hub_.emplace("/4", NcmSprite::LoadTex(L"Resources/Textures/SC/max_num.png"));
	ui_hub_.emplace("bar", NcmSprite::LoadTex(L"Resources/Textures/SC/bar.png"));
	ui_hub_.emplace("space", NcmSprite::LoadTex(L"Resources/Textures/SC/space.png"));
	bar_size_ = NcmSprite::GetSize(ui_hub_["bar"]);
	NcmSprite::SetAnchorPoint(ui_hub_["space"], { 0.5f, 1.0f });

	for (auto &i : hp_)
	{
		i = NcmSprite::LoadTex(L"Resources/white1x1.png");
	}
}

int NcmUi::GetHandle(std::string name)
{
	return ui_hub_[name];
}

void NcmUi::Draw(std::string name)
{
	NcmSprite::DrawTex(ui_hub_[name]);
}

void NcmUi::Draw(std::string name, XMFLOAT2 pos)
{
	NcmSprite::DrawTex(ui_hub_[name], pos);
}

void NcmUi::DrawMissileNumSet(int32_t num)
{
	// /4 のサイズを取得
	XMFLOAT2 size = NcmSprite::GetSize(ui_hub_["/4"]);
	// 下端を基準点に
	NcmSprite::SetAnchorPoint(ui_hub_["/4"], { 0, 1.0f });

	// 描画目安点を設定
	XMFLOAT2 draw_landmark{};
	draw_landmark.x = Win32App::FSIZE_.x - size.x;
	draw_landmark.y = Win32App::FSIZE_.y - 30.0f;

	// オフセット値を設定
	XMFLOAT2 offset = { 30.0f, 30.0f };

	// 右端点を記憶
	XMFLOAT2 right_pos =
	{
		draw_landmark.x + offset.x,
		draw_landmark.y - offset.y + 10.0f,
	};
	// /4 を描画
	NcmSprite::DrawTex(ui_hub_["/4"], { draw_landmark.x - offset.x, draw_landmark.y - offset.y });

	// 入力されたnumをstringに変換
	std::string str = std::to_string(num);
	// 下端を基準点に
	NcmSprite::SetAnchorPoint(ui_hub_[str], { 0, 1.0f });

	// /4 の位置に対するパディングを設定
	float padding = -10.0f;
	// 描画目安点をテクスチャサイズに合わせて更新
	draw_landmark.x -= NcmSprite::GetSize(ui_hub_[str]).x - padding;

	// numを描画
	NcmSprite::DrawTex(ui_hub_[str], { draw_landmark.x - offset.x, draw_landmark.y - offset.y });

	// 右端を基準点に
	NcmSprite::SetAnchorPoint(ui_hub_["bar"], { 1.0f, 0 });

	// barを描画
	NcmSprite::DrawTex(ui_hub_["bar"], right_pos);
	bottom_pos_ = right_pos.y;
}

void NcmUi::DrawHp(uint32_t hp, float size)
{
	if (IsZeroOrLess(hp))
	{
		return;
	}

	XMFLOAT2 _size = NcmSprite::GetSize(ui_hub_["bar"]);
	_size.x = size;
	for (auto &i : hp_)
	{
		NcmSprite::SetSize(i, _size);
	}

	float space = 10.0f;
	XMFLOAT2 draw_landmark{};
	draw_landmark.x = 0.0f + 30.0f;
	draw_landmark.y = Win32App::FSIZE_.y - 70.0f;

	for (UINT i = 0; i < hp; i++)
	{
		float draw_pos = NcmSprite::GetSize(hp_[0]).x * i + draw_landmark.x;
		NcmSprite::DrawTex(hp_[i], XMFLOAT2(draw_pos + space * i, bottom_pos_));
	}
}

void NcmUi::DrawSpace()
{
	NcmSprite::DrawTex(ui_hub_["space"], XMFLOAT2(Win32App::FCENTER_.x, bottom_pos_));
}

void NcmUi::CalcBarSize(int32_t count, int32_t max_charge_time)
{
	static int32_t dirty = max_charge_time;
	XMFLOAT2 size = bar_size_;

	if (dirty != count)
	{
		// いったん0~1基準に変換
		size.x /= max_charge_time;
		// max_charge_time基準に変換
		size.x *= count;
		dirty = count;
	}

	NcmSprite::SetSize(ui_hub_["bar"], size);
}
