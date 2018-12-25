#pragma once

#ifndef __AIM_HPP
#define __AIM_HPP

#include "SDK.h"
#include "Maths.hpp"
#include "CBasePlayer.hpp"
#include "Prediction.hpp"
#include "Backtracking.hpp"
#include "GameEvents.hpp"
#include "Utils.hpp"
#include <public/steam/isteamfriends.h>
#include <public/steam/isteamuser.h>
#include <vstdlib/random.h>

enum hitbox
{
	head = 0,
	pelvis = 1,
	spine_0 = 2,
	spine_1 = 3,
	spine_2 = 4,
	spine_3 = 5,
	upperArm_L = 6,
	lowerArm_L = 7,
	hand_L = 8,
	upperArm_R = 9,
	lowerArm_R = 10,
	hand_R = 11,
	hip_L = 12,
	knee_L = 13,
	foot_L = 14,
	hip_R = 15,
	knee_R = 16,
	foot_R = 17,
};

class CAim
{
public:
	void run(CUserCmd* cmd);
};

extern CAim g_Aim;
#endif