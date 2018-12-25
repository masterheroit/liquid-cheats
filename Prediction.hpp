#pragma once

#ifndef __PRED_SYS_HPP
#define __PRED_SYS_HPP
#include "CBasePlayer.hpp"
#include "bsp.hpp"
#include "Prediction.h"

class C_BasePrediction008
{
public:
	void runSimulation(IPrediction *pred, int curr_cmd, float currTime, CUserCmd *cmd, CBaseEntity *pBaseEnt);
	void start(c_baseplayer* m_pEnt, CUserCmd* m_pUssr);
	void enddh(c_baseplayer* m_pEnt, CUserCmd* m_pUssr, float oc, float of);
};

class G_Prediction
{
public:
	void start_prediction(c_baseplayer* local, CUserCmd* pCommand);
	void end_prediction(c_baseplayer* local, float original_curtime, float original_frametime);
};

class C_PredictionData008
{
public:
	Vector old_origin;
	Vector new_origin;
	Vector new_eyepos;

	float old_curtime;
	int old_conds;
	int old_flags;
};

extern C_BasePrediction008 g_pBasePrediction;
extern C_PredictionData008 g_pPredData008;

#endif