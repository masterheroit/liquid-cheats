#pragma once

#ifndef __utldm
#define __utldm

#include "SDK.h"
#include "CBasePlayer.hpp"
#include "Maths.hpp"

namespace utldm {
	bool barrel_in_range(QAngle ang_local, float barrel_dst);
	void predict_barrel(CUserCmd* cmd);
}

#endif