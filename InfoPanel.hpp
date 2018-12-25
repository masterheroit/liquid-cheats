#pragma once

#include "SDK.h"
#include "CDrawManager.h"
#include <iserver.h>
#include "CBasePlayer.hpp"
#include <string>

class c_info_panel
{
public:
	void paint_traverse();

	template < class T >
	const char* string_transform(T t)
	{
		return (const char*)(std::to_string(t).c_str());
	}

	float cur_speed;
	float max_speed;

	static int pitch;
	static int yaw;

	int i;
};

extern c_info_panel g_info_panel;