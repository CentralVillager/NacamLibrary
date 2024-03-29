#pragma once
#include "../../Lib/DebugUtill/NcmImGui.h"

class AbsScene
{
public:

	AbsScene() = default;
	virtual ~AbsScene() = default;

	/// <summary>
	/// 各シーンの持つオブジェクトの初期化処理
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ポストエフェクト処理後の描画処理
	/// </summary>
	virtual void AfterPostEffectDraw() = 0;

	/// <summary>
	/// デバッグ描画
	/// </summary>
	virtual void DebugDraw() = 0;
};
