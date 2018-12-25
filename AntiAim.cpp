#include "AntiAim.hpp"
#include "NetChannel.hpp"
#include "Maths.hpp"
#include "CBasePlayer.hpp"
#include <algorithm>
#include <array>
#include <random>
#include <vstdlib/random.h>
#include <inetchannel.h>
#include <stdlib.h>
#include <stdio.h>

#define _breakif else if

CAntiAims g_AntiAims;

float Plat_SafeAngle(float randomfloat)
{
	Maths::Clamp(randomfloat);

	return randomfloat;
}

/*
	358.f = Real: Center | Fake: Up
	434.f = Real: Down | Fake: Up
	655.f = Real: Half Up | Fake: Down
	1041.f = Real: Center | Fake: Down
	1127.f = Real: Down | Fake: Up
*/

void anti_lmaobox(CUserCmd* cmd, int mode)
{
	if (mode == 0) //Fake- / Fake (Up)
	{
		static bool flip = false;

		if (flip) cmd->viewangles.x = 434.f;
		else cmd->viewangles.x = -271.f;

		flip = !flip;
	}

	if (mode == 1) //Fake- / Fake (Down)
	{
		static bool flip = false;

		if (flip) cmd->viewangles.x = -434.f;
		else cmd->viewangles.x = -271.f;

		flip = !flip;
	}
}

// Initialize Edge vars
float edgeYaw = 0;
float edgeToEdgeOn = 0;

// Function to return distance from you to a yaw directed to
float edgeDistance(float edgeRayYaw)
{
	auto local = GetBasePlayer(me);

	// Main ray tracing area
	trace_t trace;
	Ray_t ray;
	CTraceFilter f;
	Vector forward;
	float sp, sy, cp, cy;
	sy = sinf(DEG2RAD(edgeRayYaw)); // yaw
	cy = cosf(DEG2RAD(edgeRayYaw));
	sp = sinf(DEG2RAD(0)); // pitch
	cp = cosf(DEG2RAD(0));
	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
	forward = forward * 300.0f + local->EyePosition();
	ray.Init(local->EyePosition(), forward);
	// trace::g_pFilterNoPlayer to only focus on the enviroment
	I::Trace->TraceRay(ray, 0x4200400B, NULL, &trace);
	// Pythagorean theorem to calculate distance
	float edgeDistance = (sqrt(pow(trace.startpos.x - trace.endpos.x, 2) +
		pow(trace.startpos.y - trace.endpos.y, 2)));
	return edgeDistance;
}

// Function to Find an edge and report if one is found at all
bool findEdge(float edgeOrigYaw)
{
	// distance two vectors and report their combined distances
	float edgeLeftDist = edgeDistance(edgeOrigYaw - 21);
	edgeLeftDist = edgeLeftDist + edgeDistance(edgeOrigYaw - 27);
	float edgeRightDist = edgeDistance(edgeOrigYaw + 21);
	edgeRightDist = edgeRightDist + edgeDistance(edgeOrigYaw + 27);

	// If the distance is too far, then set the distance to max so the angle
	// isnt used
	if (edgeLeftDist >= 260)
		edgeLeftDist = 999999999;
	if (edgeRightDist >= 260)
		edgeRightDist = 999999999;

	// If none of the vectors found a wall, then dont edge
	if (edgeLeftDist == edgeRightDist)
		return false;

	// Depending on the edge, choose a direction to face
	if (edgeRightDist < edgeLeftDist)
	{
		edgeToEdgeOn = 1;
		// Correction for pitches to keep the head behind walls
		if (((int)gOpts.anti_aims_pitch_mode == 3))
			edgeToEdgeOn = 2;

		if (((int)gOpts.anti_aims_pitch_mode == 4))
			edgeToEdgeOn = 1;

		return true;
	}
	else
	{
		edgeToEdgeOn = 2;
		if (((int)gOpts.anti_aims_pitch_mode == 3))
			edgeToEdgeOn = 1;

		if (((int)gOpts.anti_aims_pitch_mode == 4))
			edgeToEdgeOn = 2;
		return true;
	}
}

// Function to give you a static angle to use
float useEdge(float edgeViewAngle)
{
	// Var to be disabled when a angle is choosen to prevent the others from
	// conflicting
	bool edgeTest = true;
	if (((edgeViewAngle < -135) || (edgeViewAngle > 135)) && edgeTest == true)
	{
		if (edgeToEdgeOn == 1)
			edgeYaw = (float)-90;
		if (edgeToEdgeOn == 2)
			edgeYaw = (float)90;
		edgeTest = false;
	}
	if ((edgeViewAngle >= -135) && (edgeViewAngle < -45) && edgeTest == true)
	{
		if (edgeToEdgeOn == 1)
			edgeYaw = (float)0;
		if (edgeToEdgeOn == 2)
			edgeYaw = (float)179;
		edgeTest = false;
	}
	if ((edgeViewAngle >= -45) && (edgeViewAngle < 45) && edgeTest == true)
	{
		if (edgeToEdgeOn == 1)
			edgeYaw = (float)90;
		if (edgeToEdgeOn == 2)
			edgeYaw = (float)-90;
		edgeTest = false;
	}
	if ((edgeViewAngle <= 135) && (edgeViewAngle >= 45) && edgeTest == true)
	{
		if (edgeToEdgeOn == 1)
			edgeYaw = (float)179;
		if (edgeToEdgeOn == 2)
			edgeYaw = (float)0;
		edgeTest = false;
	}
	// return with the angle choosen
	return edgeYaw;
}

void break_lby(float real, float fake_push, Vector vel, CUserCmd* cmd) {
	static int tick = 0;

	auto time = (float)0;

	if (vel.x >= 7.01f) {
		time = 0.22f;
	}
	else time = 10.1f;

	if (tick < time) {
		cmd->viewangles.y += real;
		++tick;
		G::SendPackets = false;
	}
	else {
		G::SendPackets = true;
		cmd->viewangles.y -= real;
		tick = 0;
	}
}

void fake_yaw(float interval, float& ang, float real, float fake)
{
	static bool bFlip = false;

	if (bFlip == true) {
		G::SendPackets = true;
		ang = fake;
	} _breakif(bFlip == false) {
		G::SendPackets = false;
		ang = real;
		G::SendPackets = true;
	}

	bFlip != bFlip;
}

void CAntiAims::run(bool& bsp, CUserCmd* cmd)
{
	auto local = GetBasePlayer(me);

	if (!local) return;

	if (!cmd || cmd->buttons & IN_ATTACK || !gOpts.anti_aims_active || !bsp)
		return;

	auto chan = (INetChannel*)I::Engine->GetNetChannelInfo();

	if (!chan)
		return;

	float of;
	float os;
	Vector ov;

	Maths::BackupSf(cmd, of, os, ov);

	Vector& new_angles = cmd->viewangles;

	if (gOpts.anti_aims_pitch_mode == 1) {
		new_angles.x = -89.0f;
	} _breakif(gOpts.anti_aims_pitch_mode == 2) {
		new_angles.x = 89.0f;
	} _breakif(gOpts.anti_aims_pitch_mode == 3) {
		new_angles.x = -271.0f;
	} _breakif(gOpts.anti_aims_pitch_mode == 4) {
		new_angles.x = 271.0f;
	} _breakif(gOpts.anti_aims_pitch_mode == 5) {
		anti_lmaobox(cmd, 0);
	} _breakif(gOpts.anti_aims_pitch_mode == 6) {
		anti_lmaobox(cmd, 1);
	}

	float fmov = sqrt((cmd->forwardmove*cmd->forwardmove) + (cmd->sidemove*cmd->sidemove) + (cmd->upmove*cmd->upmove));
	if (fmov > 1000.0f && fmov > 0.0)
	{
		float fratio = 1000.0f / fmov;
		cmd->forwardmove *= fratio;
		cmd->sidemove *= fratio;
		cmd->upmove *= fratio;
	}
	
	I::CVars->FindVar("cl_yawspeed")->SetValue(10000.0f);

	if (gOpts.anti_aims_yaw_mode == 1) {
		new_angles.y -= 90;
	} _breakif(gOpts.anti_aims_yaw_mode == 2) {
		new_angles.y += 90;
	} _breakif(gOpts.anti_aims_yaw_mode == 3) {
		break_lby(90, 180, Vector{ *(float*)(local + 0xF4), 0.f, 0.f }, cmd);
	} _breakif(gOpts.anti_aims_yaw_mode == 4) {
		break_lby(-90, 180, Vector{ *(float*)(local + 0xF4), 0.f, 0.f }, cmd);
	} _breakif(gOpts.anti_aims_yaw_mode == 5) {
		if (findEdge(cmd->viewangles.y)) {
			static float lby = 0.0f;

			if (lby < 1.1f) {
				lby += 0.1;
				cmd->viewangles.y = useEdge(cmd->viewangles.y);
			}
			else if(lby < 1.2f){
				cmd->viewangles.y -= useEdge(cmd->viewangles.y);
				lby += 0.1;
			}
			else {
				lby = 0;
			}
		}
	}

	Maths::FixSf(cmd, ov, of, os);
}