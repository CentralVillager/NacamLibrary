#pragma once
#include <memory>
#include "AbsScene.h"
#include "../../Lib/2D/NcmSprite.h"

class ResultScene : public AbsScene
{
	//std::unique_ptr<NcmSprite> result_;
	int result_;

public:

	ResultScene();
	~ResultScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void AfterPostEffectDraw() override;
	void DebugDraw() override;
};
