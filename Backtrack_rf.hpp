#pragma once

#ifndef __BACKTRACK_RF_HPP
#define __BACKTRACK_RF_HPP

#include "SDK.h"

class C_Backtrackrf
{
public:
	void run(CUserCmd* cmd);
};

extern C_Backtrackrf g_BacktrackRf;
#endif