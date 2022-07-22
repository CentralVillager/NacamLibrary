#pragma once
#include "Model.h"
#include "Object3d.h"
#include <memory>

class Player {

	std::shared_ptr<Model> model_;
	std::shared_ptr<Object3d> object_;

public:

	Player();
	~Player();

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();
};

