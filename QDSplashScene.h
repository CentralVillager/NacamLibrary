#pragma once
#include "AbstractScene.h"
#include "Sprite.h"

class QDSplashScene : public AbstractScene {

	// ���S
	unique_ptr<Sprite> logo_;

	// ���S�̕\������
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
