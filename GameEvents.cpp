#include "GameEvents.hpp"
#include <igameevents.h>

GameEvents g_GameEvents_manager;
CGameEvents g_GameEvents;

void GameEvents::FireGameEvent(IGameEvent* p_Event)
{
	if (!strcmp(p_Event->GetName(), "player_hurt"))
	{
		int attacker_t = p_Event->GetInt("attacker");
		int victim_t = p_Event->GetInt("userid");

		auto attacker = I::Engine->GetPlayerForUserId(attacker_t);
		auto victim = I::Engine->GetPlayerForUserId(victim_t);

		if (attacker == I::Engine->GetLocalPlayer())
		{
			g_GameEvents.shouldincrementresolvermode[victim] = true;
		}
	}
}

void GameEvents::Init()
{
	if (I::EventManager->FindListener(this, "player_hurt") != true)
		I::EventManager->AddListener(this, "player_hurt", false);
}