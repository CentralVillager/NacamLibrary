#include "EmittersList.h"

EmittersList::EmittersList()
{
	emitters_.resize(0);
}

EmittersList::~EmittersList()
{}

void EmittersList::Update(const XMFLOAT3 &pos)
{
	for (auto &i : emitters_)
	{
		i.SetPosition(pos);
		i.GenerateParticle();
		i.UpdateParticle();
	}

	emitters_.remove_if([](Emitter &x) { return x.NoticeCanTerminate(); });
}

void EmittersList::Draw()
{
	for (auto &i : emitters_)
	{
		i.Draw();
	}
}

void EmittersList::DebugDraw()
{}

void EmittersList::Add(const EmitterDesc &args)
{
	emitters_.emplace_front();
	emitters_.front().SetEmitterDesc(args);
}
