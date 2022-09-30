#pragma once
#include <memory>
#include "AbstractScene.h"
#include "../Sprite/Sprite.h"

class ResultScene : public AbstractScene
{
	//std::unique_ptr<Sprite> result_;
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
