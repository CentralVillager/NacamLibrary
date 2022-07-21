#pragma once
#include "AbstractScene.h"
#include "Sprite.h"

class QDSplashScene : public AbstractScene {

	// ƒƒS
	unique_ptr<Sprite> logo_;

	// ƒƒS‚Ì•\¦ŠÔ
	int display_time_;

public:

	QDSplashScene();
	~QDSplashScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
