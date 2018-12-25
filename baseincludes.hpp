#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <d3dx9.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//#define _HL2DM_MP_v1

#define _DWORD uint32
#define LODWORD(x)  (*((_DWORD*)&(x)))  // low dword

class Activity;

#define __vpad(t, x) virtual t x() = 0; 

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include <math.h>
#include <xstring>
#include "getvfunc.h"
#include "dt_recv2.h"
#include "CSignature.h"
#include "WeaponList.h"
#include "CGlobalVars.h"
#include "VMTHooks.h"
#include "Log.h"
#include "NetPtr.hpp"
#include "Color.hpp"
#include <cdll_int.h>
#include <strtools.h>
#include <tier0/dbg.h>
#include <tier1/iconvar.h>
#include <icvar.h>
#include <tier1/utlvector.h>
#include <vstdlib/random.h>
#include <vstdlib/random.h>
#include <inetmsghandler.h>
#include <convar.h>
#include <igameevents.h>
#include <steam/isteamclient.h>
#include <iprediction.h>
#include "testentity.hpp"
#include <engine/ivmodelinfo.h>
#include <string>

#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "materialsystem/imaterialvar.h"
#include "materialsystem/itexture.h"
#include "model_types.h"
#include "tier1/keyvalues.h"

#include <game/client/ScreenSpaceEffects.h>
#include <game/shared/ehandle.h>
#include <con_nprint.h>

#pragma comment(lib, "appframework.lib")
#pragma comment(lib, "steam_api.lib")
#pragma comment(lib, "tier0.lib")
#pragma comment(lib, "tier1.lib")
#pragma comment(lib, "raytrace.lib")
#pragma comment(lib, "tier2.lib")
#pragma comment(lib, "tier3.lib")
#pragma comment(lib, "mathlib.lib")
#pragma comment(lib, "vstdlib.lib")
#pragma comment(lib, "matsys_controls.lib")
#pragma comment(lib, "vtf.lib")
#pragma comment(lib, "particles.lib")
#pragma comment(lib, "bitmap.lib")
#pragma comment(lib, "2010\\libprotobuf.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

using namespace std;

typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

#define VMTManager toolkit::VMTManager
#define VMTBaseManager vfunc_hook

#define WIN32_LEAN_AND_MEAN
#pragma optimize("gsy",on)
#pragma warning( disable : 4244 ) //Possible loss of data

typedef float matrix3x4[3][4];

struct trace_data_t
{
	float fraction;
	float fraction_leftsolid;

	Vector startpos;
	Vector endpos;
};

#define me I::Engine->GetLocalPlayer()
#define GetBaseEntity I::EntList->GetClientEntity
#define MASK_AIMBOT 0x200400B
#define	FL_ONGROUND (1<<0)
#define FL_DUCKING (1<<1)
#define CONTENTS_HITBOX 0x40000000
#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define square( x ) ( x * x )
#define RADPI 57.295779513082f
#define SQUARE( a ) a*a
#define BLU_TEAM 3
#define RED_TEAM 2