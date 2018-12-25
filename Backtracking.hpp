#pragma once

#ifndef __BACKTRACK_HPP
#define __BACKTRACK_HPP

#include "SDK.h"
#include "CBasePlayer.hpp"

inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}

class CHitpos
{
public:
	Vector ptr;
	int tick;
	float curtime;
};

class CRaw
{
public:
	c_baseplayer * hEntity[34];
	CHitpos hitpos[34][12];
};

class CData
{
public:
	CRaw* data;
	int bttick;
	void run(CUserCmd* cmd);
};

extern CData* g_Btdata;
#endif