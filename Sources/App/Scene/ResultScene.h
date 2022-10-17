#pragma once
#include <memory>
#include "AbstractScene.h"
#include "../../Lib/Sprite/NcmSprite.h"

class ResultScene : public AbstractScene
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
	void DebugDraw() override;
};
