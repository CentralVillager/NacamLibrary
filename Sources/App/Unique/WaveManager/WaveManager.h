#pragma once
#include <DirectXMath.h>
#include "../Enemy/EnemiesList.h"

class WaveManager
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	enum class MoveType
	{

	};

private:

	UINT current_wave_;
	UINT wave_enemy_num_;

	EnemiesList *ene_list_;

public:

	WaveManager();
	~WaveManager();

public:

	void Initialize(EnemiesList *ene_list);
	void Finalize();
	void Update();

public:

	void SetWave(UINT wave_num);
};
