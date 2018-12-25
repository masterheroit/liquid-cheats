#pragma once

#ifndef VISUALS_H
#define VISUALS_H

#include "SDK.h"
#include "CBasePlayer.hpp"
#include "CDrawManager.h"
#include "UTLDM.hpp"

class CVisuals
{
public:
	void run();
};

class C_HL2DMVisuals {
public:
	void run();
	void visuals(c_baseplayer* entity);
	void local_visuals(c_baseplayer* entity);
	void barrel_visuals(c_baseplayer* entity);
};

extern CVisuals g_Visuals;

#endif

void CreateVisuals(c_baseplayer * entity);

namespace HL2DM {

	extern C_HL2DMVisuals g_Visuals;
}