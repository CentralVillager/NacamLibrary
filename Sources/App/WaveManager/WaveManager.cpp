#include "WaveManager.h"

WaveManager::WaveManager()
{}

WaveManager::~WaveManager()
{}

void WaveManager::Initialize(EnemiesList *ene_list)
{
	ene_list_ = ene_list;
}

void WaveManager::Finalize()
{}

void WaveManager::Update()
{}

void WaveManager::SetWave(UINT wave_num)
{
	switch (wave_num)
	{
	case 1:
		ene_list_->Add(XMFLOAT3(-30.0f, 0, 50.0f));
		ene_list_->Add(XMFLOAT3(0, 0, 50.0f));
		ene_list_->Add(XMFLOAT3(30.0f, 0, 50.0f));
		break;

	case 2:
		ene_list_->Add(XMFLOAT3(30.0f, 0, 50.0f));
		break;
	}
}
