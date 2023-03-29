#pragma once
#include "../Singleton/Singleton.h"

class NcmDebug : public Singleton<NcmDebug>
{
	friend Singleton<NcmDebug>;

	bool debug_mode_ = false;
	bool cheat_mode_ = false;

public:

	bool IsDebugMode() { return debug_mode_; }
	bool IsCheatMode() { return cheat_mode_; }
	const void SetDebugMode(bool mode) { debug_mode_ = mode; }
	const void SetCheatMode(bool mode) { cheat_mode_ = mode; }
};
