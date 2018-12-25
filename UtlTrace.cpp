#include "UtlTrace.hpp"
#include "Maths.hpp"
#include "SDK.h"

namespace TraceUtils
{
	CTraceData CalculateTraceData(Vector vStart)
	{
		CGameTrace pTrace;

		Ray_t pRay;

		CTraceFilter pFilter;

		auto local = GetBaseEntity(me);

		pFilter.pSkip = local;

		//OK;
		Vector anglevectors;

		Maths::AngleVectors(vStart, &anglevectors);

		anglevectors *= 10000;

		for (int i = 0; i <= 1000000; i++)
		{
			pRay.Init(vStart, anglevectors);

			I::Trace->TraceRay(pRay, MASK_SHOT_HULL, &pFilter, &pTrace);

			auto length = (vStart - (vStart + anglevectors)).Length();
			auto endpos = pTrace.endpos;

			auto data = CTraceData{
				length,
				vStart,
				endpos,
				NULL,
				pTrace.surface.name,
			};

			if (pTrace.allsolid)
				return data;
		}
	}
}