#pragma once

#include "SDK.h"
#include "CDrawManager.h"
#include "CBasePlayer.hpp"
#include <d3dx9.h>

class c_radar
{
public:
	void DrawRadarPoint(Vector origx, Vector origy, QAngle angle, c_baseplayer *ent);
	void DrawRadarIcon(IDirect3DDevice9* pDevice, Vector origx, Vector origy, QAngle angle, c_baseplayer *ent);
	void paint_traverse();
	static int pitch;
	static int yaw;
	static int s;
};

extern c_radar g_radar;