#pragma once
#include <DirectXMath.h>
#include <forward_list>
#include "Emitter.h"

class EmittersList
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

	std::forward_list<Emitter> emitters_;

public:

	EmittersList();
	~EmittersList();

public:

	void Update(const XMFLOAT3 &pos);
	void Draw();
	void DebugDraw();

public:

	void Add(const EmitterDesc &args);
};
