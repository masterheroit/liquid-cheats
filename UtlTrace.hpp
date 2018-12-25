/*
	*		Utltrace.hpp		*
		   Created By Bux
*/

#ifndef __UTLTRACE_HPP
#define __UTLTRACE_HPP

//Forward Declarations;
#include "SDK.h"
#include <basehandle.h>

class CTraceData
{
public:
	float dist;
	Vector vStart;
	Vector vEnd;
	CBaseHandle hEntity;
	const char* name;
};

namespace TraceUtils
{
	CTraceData CalculateTraceData(Vector vStart);
}

#endif