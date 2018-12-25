#pragma once

#include "SDK.h"
#include "CBasePlayer.hpp"
#include "NetChannel.hpp"

class Generals
{
public:
	void run(bool& bsp, CUserCmd* cmd);
};

extern Generals g_Generals;