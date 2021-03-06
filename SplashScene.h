#pragma once
#include "AbstractScene.h"
#include "Sprite.h"

class SplashScene : public AbstractScene {

	// ロゴ
	unique_ptr<Sprite> logo_;

	// ロゴの表示時間
	int display_time_;

public:

	SplashScene();
	~SplashScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
