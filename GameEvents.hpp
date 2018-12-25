#pragma once

#include "SDK.h"
#include "CBasePlayer.hpp"

class GameEvents : public IGameEventListener2
{
public:
	void FireGameEvent(IGameEvent* p_Event);
	void Init();

	inline void Unload() {
		auto ok = I::EventManager->FindListener(this, "player_hurt");

		if (ok)I::EventManager->RemoveListener(this);
	}
};

class CGameEvents
{
public:
	bool shouldincrementresolvermode[34];
	int modes[34];
};

extern GameEvents g_GameEvents_manager;
extern CGameEvents g_GameEvents;