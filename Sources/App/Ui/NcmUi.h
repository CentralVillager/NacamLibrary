#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <DirectXMath.h>
#include <array>
#include "../Number/Numbers.h"
#include "../../Lib/Sprite/NcmSprite.h"
#include "../../App/Math/Easing/NcmEasing.h"

class NcmUi
{
	using XMFLOAT2 = DirectX::XMFLOAT2;

	static std::unique_ptr<Numbers> numbers_;
	static std::unordered_map<std::string, int> ui_hub_;
	static int32_t count_;
	static XMFLOAT2 bar_size_;
	static float bottom_pos_;

	static std::array<int, 3> hp_;

	static ncm_handle ease_;

public:

	static void Initialize();
	static void LoadResources();
	static int GetHandle(std::string name);
	static void Draw(std::string name);
	static void Draw(std::string name, XMFLOAT2 pos);

public:

	static void DrawMissileNumSet(int32_t num);
	static void DrawHp(uint32_t hp, float size);
	static void DrawSpace();

public:

	static void CalcBarSize(int32_t count, int32_t max_charge_time);
};
