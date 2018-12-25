#pragma once

#include "baseincludes.hpp"

class ClientClass
{
private:
	BYTE _chPadding[8];
public:
	char* chName;
	RecvTable* Table;
	ClientClass* pNextClass;
	int iClassID;
};

class CHLClient
{
public:
	ClientClass* GetAllClasses( void )
	{
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID ); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return getvfunc<OriginalFn>( this, 8 )( this ); //Return the pointer to the head CClientClass.
	}
};

void DumpWeapons();
void DumpSig2Offs();
bool FileExists(const std::string& name);

class CGlobals
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
};

#include <game/shared/usercmd.h>

class C_AnimationLayer
{
public:
	C_AnimationLayer();
	void Reset();

	void SetOrder(int order);

public:
	bool IsActive(void);

	int   m_nSequence;
	float m_flPrevCycle;
	float m_flWeight;
	int   m_nOrder;

	float m_flPlaybackRate;
	float m_flCycle;

	float GetFadeout(float flCurTime);

	void BlendWeight();

	float m_flLayerAnimtime;
	float m_flLayerFadeOuttime;

	float m_flBlendIn;
	float m_flBlendOut;

	bool m_bClientBlend;
};

class CViewSetup
{
public:
	/*
		#  Portable CViewSetup Class #

		Created by: buxthemightyfag (me, lead developer of liquid)

		Notices:

		This class is incomplete.

		only variables that the cheat might require, have been dumped, due to time / timing issues / lack of time.
	*/
	char pad_0x0000[0x10]; //0x0000
	__int32 m_iWindowX; //0x0010 
	__int32 m_iWindowUnscaledX; //0x0014 
	__int32 m_iWindowY; //0x0018 
	char pad_0x001C[0x4]; //0x001C
	__int32 m_iWindowUnscaledY; //0x0020 
	char pad_0x0024[0x14]; //0x0024
	float m_flFov; //0x0038 
	float m_flViewmodelFov; //0x003C 
	Vector m_vOrigin; //0x0040 
	Vector m_vAngles; //0x004C 
	float m_flZNear; //0x0058 
	char pad_0x005C[0x3E4]; //0x005C

}; //Size=0x0440

class CBaseEntity
{
public:
	bool IsPlayer()
	{
		return (this->GetClientClass()->iClassID == 246);
	}
	Vector& GetAbsOrigin( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 9)(this);
	}
	int GetMaxHealth()
	{
		typedef int(__thiscall *OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 107)(this);
	}
	Vector& GetOldOrigin()
	{
		typedef Vector& (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 129)(this);
	}
	Vector& GetAbsAngles( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 10)(this);
	}
	int GetNumAnimOverlays() const
	{
		CUtlVector<C_AnimationLayer> &m_AnimOverlay = *(CUtlVector<C_AnimationLayer> *)(this + 2216);
		return m_AnimOverlay.Count();
		// return *(int *)(this + 2288); // dont ask - check for "%8.4f : %30s : %5.3f : %4.2f : %1d"
	}
	void SetSize(Vector &mins, Vector &maxs) const
	{
		static DWORD dwFunc = sigs::GetClientSignature("55 8B EC 83 EC 28 53 8B 5D 08 56 8B 75 0C 57 8B 03");
		typedef void(__thiscall * OriginalFn)(const CBaseEntity *, Vector &, Vector &);
		return ((OriginalFn)(dwFunc))((this + 456), mins, maxs); // dont question it - check for "maxs" or "mins"
	}
	matrix3x4 &GetRgflCoordinateFrame()
	{
		// NETVAR_OFF( n, DWORD, -0x4C, "DT_BaseEntity", "m_CollisionGroup" );
		// return *reinterpret_cast< matrix3x4 * >( n.getValue(this) );
		PVOID pRenderable = static_cast<PVOID>(this + 0x4);
		typedef matrix3x4 &(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pRenderable, 34)(pRenderable);
	}
	void GetWorldSpaceCenter( Vector& vWorldSpaceCenter)
	{
		Vector vMin, vMax;
		this->GetRenderBounds( vMin, vMax );
		vWorldSpaceCenter = this->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	}
	void SetAbsOrigin(Vector& ang) {
		typedef void(__thiscall * OriginalFn)(const CBaseEntity *, Vector &);
		// search for "origin" and "angles"
		static OriginalFn fn = (OriginalFn)sigs::FindSig("client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07 0F 2E 86 ? ? ? ? 9F F6 C4 44 7A 28 F3 0F 10 47 ? 0F 2E 86 ? ? ? ? 9F F6 C4 44 7A 16 F3 0F 10 47 ? 0F 2E 86 ? ? ? ? 9F F6 C4 44 0F 8B ? ? ? ? 53 6A 01 8B CE E8 ? ? ? ? 81 A6 ? ? ? ? ? ? ? ? 8D 86 ? ? ? ? D9 07 D9 9E ? ? ? ? D9 47 04 50 D9 9E ? ? ? ? D9 47 08 6A 03 57 D9 9E ? ? ? ? E8 ? ? ? ? 8B 8E ? ? ? ? 83 C4 0C 83 F9 FF 74 45 8B 1D ? ? ? ? 8B C1 25 ? ? ? ? C1 E9 0C 03 C0 39 4C C3 08 75 2D 8B 5C C3 04 85 DB 74 25 8B CB E8 ? ? ? ? 8D 86 ? ? ? ? 50 8D 83 ? ? ? ? 50 57 E8 ? ? ? ? 83 C4 0C 5B 5F");
		return fn(this, ang);
	}
	DWORD* GetModel( )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pRenderable, 9 )( pRenderable );
	}
	bool SetupBones( matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef bool ( __thiscall* OriginalFn )( PVOID, matrix3x4*, int, int, float );
		return getvfunc<OriginalFn>( pRenderable, 16 )( pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime );
	}
	ClientClass* GetClientClass( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 2 )( pNetworkable );
	}
	bool IsDormant( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 8 )( pNetworkable );
	}
	int GetIndex( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 9 )( pNetworkable );
	}
	void GetRenderBounds(Vector& mins, Vector& maxs)
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef void(__thiscall* OriginalFn)(PVOID, Vector&, Vector&);
		getvfunc<OriginalFn>(pRenderable, 20)(pRenderable, mins, maxs);
	}
};

//Game Dependant Signature
#ifdef HL2MP_DM_v1
#define __WRITEUSERCMD_SIG "55 8B EC 8B 45 10 83 EC 10 B9 ? ? ? ? 8B 40 04 53 8B 5D 08 40 56 57 8B 7D 0C 39 47 04 8B 43 0C 0F 84 ? ? ? ? 3B 43 08 7C 05 88 4B 10 EB 1A 8B 13 8B F0 83 E0 1F C1 FE 05 89 45 F8 8B C1 8B 4D F8 D3 E0 09 04 B2 FF 43 0C 8B 47 04 8B 4B 08 8B 73 0C 89 45 FC 8B C1 2B C6 83 F8 20 7D 0C 89 4B 0C C6 43 10 01 E9 ? ? ? ? 8D 46 20 8B CE 89 43 0C 8B D6 8B 03 83 E2 1F C1 F9 05 BF ? ? ? ? 8B 1C 88 89 55 F4 8D 04 88 8B CA D3 45 FC B9 ? ? ? ? 2B CA 89 45 F8 D3 EF 8B 4D F4 8B F7 83 E6 01 8B 04 B0 8B D0 33 55 FC 23 D7 8B 7D F8 33 D0 83 C8 FF D3 E0 89 14 B7 8B D3 33 55 FC 23 D0 33 D3 8B 5D 08 89 17 8B 7D 0C EB 26 3B 43 08 7C 05 88 4B 10 EB 1C 8B 13 8B F0 83 E0 1F C1 FE 05 89 45 F4 8B C1 8B 4D F4 D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 45 10 8B 4B 0C 8B 40 08 40 39 47 08 0F 84 ? ? ? ? 3B 4B 08 7C 06 C6 43 10 01 EB 17 8B 13 8B F1 C1 FE 05 83 E1 1F B8 ? ? ? ? D3 E0 09 04 B2 FF 43 0C 8B 47 08 8B 4B 08 8B 73 0C 89 45 FC 8B C1 2B C6 83 F8 20 7D 0C 89 4B 0C C6 43 10 01 E9 ? ? ? ? 8D 46 20 8B CE 89 43 0C 8B D6 8B 03 83 E2 1F C1 F9 05 BF ? ? ? ? 8B 1C 88 89 55 F8 8D 04 88 8B CA D3 45 FC B9 ? ? ? ? 2B CA 89 45 F4 D3 EF 8B 4D F8 8B F7 83 E6 01 8B 04 B0 8B D0 33 55 FC 23 D7 8B 7D F4 33 D0 83 C8 FF D3 E0 89 14 B7 8B D3 33 55 FC 23 D0 33 D3 8B 5D 08 89 17 8B 7D 0C EB 24 3B 4B 08 7C 06 C6 43 10 01 EB 19 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 45 10 F3 0F 10 47 ? 8B 4B 0C 0F 2E 40 0C 9F F6 C4 44 7B 40 3B 4B 08 7C 14 C6 43 10 01 D9 47 0C 51 8B CB D9 1C 24 E8 ? ? ? ? EB 4B 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 51 09 04 B2 8B CB FF 43 0C D9 47 0C D9 1C 24 E8 ? ? ? ? EB 24 3B 4B 08 7C 06 C6 43 10 01 EB 19 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 45 10 F3 0F 10 47 ? 8B 4B 0C 0F 2E 40 10 9F F6 C4 44 7B 40 3B 4B 08 7C 14 C6 43 10 01 D9 47 10 51 8B CB D9 1C 24 E8 ? ? ? ? EB 4B 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 51 09 04 B2 8B CB FF 43 0C D9 47 10 D9 1C 24 E8 ? ? ? ? EB 24 3B 4B 08 7C 06 C6 43 10 01 EB 19 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 45 10 F3 0F 10 47 ? 8B 4B 0C 0F 2E 40 14 9F F6 C4 44 7B 40 3B 4B 08 7C 14 C6 43 10 01 D9 47 14 51 8B CB D9 1C 24 E8 ? ? ? ? EB 4B 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 51 09 04 B2 8B CB FF 43 0C D9 47 14 D9 1C 24 E8 ? ? ? ? EB 24 3B 4B 08 7C 06 C6 43 10 01 EB 19 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 45 10 F3 0F 10 47 ? 8B 4B 0C 0F 2E 40 18 9F F6 C4 44 7B 40 3B 4B 08 7C 14 C6 43 10 01 D9 47 18 51 8B CB D9 1C 24 E8 ? ? ? ? EB 4B 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 51 09 04 B2 8B CB FF 43 0C D9 47 18 D9 1C 24 E8 ? ? ? ? EB 24 3B 4B 08 7C 06 C6 43 10 01 EB 19 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 45 10 F3 0F 10 47 ? 8B 4B 0C 0F 2E 40 1C 9F F6 C4 44 7B 40 3B 4B 08 7C 14 C6 43 10 01 D9 47 1C 51 8B CB D9 1C 24 E8 ? ? ? ? EB 4B 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 51 09 04 B2 8B CB FF 43 0C D9 47 1C D9 1C 24 E8 ? ? ? ? EB 24 3B 4B 08 7C 06 C6 43 10 01 EB 19 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 45 10 F3 0F 10 47 ? 8B 4B 0C 0F 2E 40 20 9F F6 C4 44 7B 40 3B 4B 08 7C 14 C6 43 10 01 D9 47 20 51 8B CB D9 1C 24 E8 ? ? ? ? EB 4B 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 51 09 04 B2 8B CB FF 43 0C D9 47 20 D9 1C 24 E8 ? ? ? ? EB 24 3B 4B 08 7C 06 C6 43 10 01 EB 19 8B 13 8B F1 83 E1 1F C1 FE 05 B8 ? ? ? ? D3 E0 F7 D0 21 04 B2 FF 43 0C 8B 4D 10 8B 47 24 3B 41 24 8B 4B 0C 0F 84 ? ? ? ? 3B 4B 08 7C 06 C6 43 10 01 EB 17 8B 13 8B F1 C1 FE 05 83 E1 1F B8 ? ? ? ? D3 E0 09 04 B2 FF 43 0C 8B 4F 24 8B 73 0C 89 4D FC 8B 4B 08 8B C1 2B C6 83 F8 20 7D 09 89 4B 0C C6 43 10 01 EB 7F 8D 46 20 8B CE 89 43 0C 8B D6 8B 03 83 E2 1F C1 F9 05 8B 3C 88"
#else
#define __WRITEUSERCMD_SIG "55 8B EC 83 EC 08 53 8B 5D 0C 56 8B 75 10 57 8B 7D 08 8B 46 04 40 39 43 04 8B 47 0C 0F 84 ? ? ? ? 3B 47 08 7C 06 C6 47 10 01 EB 1E 8B 0F 8B D0 C1 FA 05 83 E0 1F 89 45 FC 8B F0 8B 04 91 0F AB F0 8B 75 10 89 04 91 FF 47 0C 8B 43 04"
#endif

#include "studiohdr.hpp"

/*class IVModelInfo
{
public:
	DWORD * GetModel(int index)
	{
		typedef DWORD*(__thiscall* GetModelFn)(void*, int);
		return getvfunc<GetModelFn>(this, 1)(this, index);
	}

	int	GetModelIndex(const char* name)
	{
		typedef int(__thiscall* GetModelIndexFn)(void*, const char*);
		return getvfunc< GetModelIndexFn >(this, 2)(this, name);
	}

	const char* GetModelName(const DWORD* model)
	{
		typedef const char* (__thiscall* GetModelNameFn)(void*, const DWORD*);
		return getvfunc< GetModelNameFn >(this, 3)(this, model);
	}

	studiohdr_t* GetStudiomodel(const DWORD *mod)
	{
		typedef studiohdr_t* (__thiscall* GetStudiomodelFn)(void*, const DWORD*);
		return getvfunc< GetStudiomodelFn >(this, 28)(this, mod);
	}
};*/

class CUnknown {};

class CVerifiedUserCmd
{
public:
	CUserCmd m_cmd;
	CRC32_t  m_crc;
};

#ifndef _HL2DM_MP_v1
class IInput
{
public:
	CUnknown* unk01; //0x0000 
	char pad_0x0004[0xA8]; //0x0004
	float m_flEyeanglesX; //0x00AC 
	float m_flEyeanglesY; //0x00B0 
	char pad_0x00B4[0x4]; //0x00B4
	float m_flLastForwardmove; //0x00B8 
	char pad_0x00BC[0x40]; //0x00BC
	CUserCmd* m_pCommands; //0x00FC 
	CVerifiedUserCmd* m_pVerifiedCommands; //0x0100 
	char pad_0x0104[0x14]; //0x0104
	ConVar* m_pCartCamFollow; //0x0118 
	char pad_0x011C[0x28]; //0x011C
	float m_flCartCamFollow; //0x0144 
	char pad_0x0148[0x158]; //0x0148
	ConVar* m_pItemInspectModelSpinRate; //0x02A0 
	char pad_0x02A4[0x2590]; //0x02A4

}; //Size=0x2834

class CInput : public IInput
{
public:
	CUserCmd* GetUserCmd(int seq)
	{
		typedef CUserCmd *(__thiscall * OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 8)(this, seq);
	}
	bool CAM_IsThirdPerson()
	{
		typedef bool(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 31)(this);
	}
};

#else

class IInput
{
public:
	char pad_0x0000[0x4]; //0x0000
	unsigned char m_bMouseInit; //0x0004 
	unsigned char m_bMouseState; //0x0005 
	char pad_0x0006[0xA]; //0x0006
	float m_flAccumulatedMouseXMovement; //0x0010 
	float m_flAccumulatedMouseYMovement; //0x0014 
	char pad_0x0018[0x8C]; //0x0018
	float m_flPreviousAnglesX; //0x00A4 
	float m_flPreviousAnglesY; //0x00A8 
	char pad_0x00AC[0x18]; //0x00AC
	CUserCmd* m_pCmds; //0x00C4 
	CVerifiedUserCmd* m_pVfCmds; //0x00C8 
	char pad_0x00CC[0x7B4]; //0x00CC

}; //Size=0x0880
class CInput : public IInput{
public:
	CUserCmd* GetUserCmd(int seq)
	{
		typedef CUserCmd *(__thiscall * OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 8)(this, seq);
	}
	bool CAM_IsThirdPerson()
	{
		typedef bool(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 31)(this);
	}
};
#endif

class EngineClient
{
public:
	void GetScreenSize( int& width, int& height )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int& , int& );
		return getvfunc<OriginalFn>( this, 5 )( this, width, height );
	}
	int GetAppId()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 104)(this);
	}
	void ClientCmdCurrentTick(const char* cmd)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const char*);
		return getvfunc<OriginalFn>(this, 102)(this, cmd);
	}
	bool GetPlayerInfo( int ent_num, player_info_t *pinfo )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID, int, player_info_t * );
		return getvfunc<OriginalFn>(this, 8)(this, ent_num, pinfo );
	}
	bool Con_IsVisible( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 11 )( this );
	}
	int GetLocalPlayer( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 12 )( this );
	}
	float Time( void )
	{
		typedef float ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 14 )( this );
	}
	float Plat_FloatTime()
	{
		typedef float(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 13)(this);
	}
	void GetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 19 )( this, va );
	}
	void SetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 20 )( this, va );
	}
	int GetMaxClients( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 21 )( this );
	}
	int GetPlayerForUserId(int uuid)
	{
		typedef int(__thiscall* OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 9)(this, uuid);
	}
	bool IsInGame( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 26 )( this );
	}
	bool IsConnected( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 27 )( this );
	}
	bool IsDrawingLoadingImage( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 28 )( this );
	}
	const matrix3x4& WorldToScreenMatrix( void )
	{
		typedef const matrix3x4& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 36)(this);
	}
	bool IsTakingScreenshot( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 85 )( this );
	}
	DWORD* GetNetChannelInfo( void )
	{
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 72 )( this );
	}
	void ClientCmd_Unrestricted( const char* chCommandString )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, const char * );
		return getvfunc<OriginalFn>( this, 106 )( this, chCommandString );
	}
};
class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall* OriginalFn)(PVOID, unsigned int);
		return getvfunc<OriginalFn>(this, 36)(this, vguiPanel);
	}
	void SetMouseInputEnabled(int vpanel, bool b)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, bool);
		getvfunc<OriginalFn>(this, 32)(this, vpanel, b);
	}
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,		// custom generated font - never fall back to asian compatibility mode
	FONTFLAG_BITMAP = 0x800,		// compiled bitmap font - no fallbacks
};

class ISurface
{
public:
	void DrawGetTextureSize(int id, int &wide, int &tall)
	{
		typedef void(__thiscall* oFn)(PVOID, int, int&, int&);
		getvfunc<oFn>(this, 33)(this, id, wide, tall);
	}
	void DrawSetTextureFile(int id, char const* filename, int hardwareFilter, bool forceReload)
	{
		typedef void(__thiscall* oFn)(PVOID, int , char const*, int , bool);
		getvfunc<oFn>(this, 30)(this, id, filename, hardwareFilter, forceReload);
	}
	void DrawTexturedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oFn)(PVOID, int, int, int, int);
		getvfunc<oFn>(this, 33)(this, x0, y0, x1, y1);
	}
	void SurfaceGetCursorPos(int& x, int& y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int&, int&);
		getvfunc<OriginalFn>(this, 96)(this, x, y);
	}
	void DrawSetTexture(int i)
	{
		typedef void(__thiscall* oFn)(PVOID, int);
		getvfunc<oFn>(this, 32)(this, i);
	}
	int CreateNewTextureId(bool procedural)
	{
		typedef int(__thiscall* oFn)(PVOID, bool);
		return getvfunc<oFn>(this, 37)(this, procedural);
	}
	bool IsTextureIdValid(int id)
	{
		typedef bool(__thiscall* oFn)(PVOID, int);
		return getvfunc<oFn>(this, 35)(this, id);
	}
	void DrawSetTextureRgba(int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload)
	{
		typedef void(__thiscall* oFn)(PVOID, int, unsigned char const*, int, int, int, bool);
		getvfunc<oFn>(this, 31)(this, id, rgba, wide, tall, hardwareFilter, forceReload);
	}
	void DrawLine(int xb, int yb, int xe, int ye)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 15)(this, xb, yb, xe, ye);
	}
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 11)(this, r, g, b, a);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 12)(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 14)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		getvfunc<OriginalFn>(this, 17)(this, font);
	}
	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 19)(this, r, g, b, a);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		getvfunc<OriginalFn>(this, 20)(this, x, y);
	}
	void DrawPrintText(const wchar_t *text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t *, int, int);
		return getvfunc<OriginalFn>(this, 22)(this, text, textLen, 0);
	}
	unsigned long CreateFont()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 66)(this);
	}
	void SetFontGlyphSet(unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		getvfunc<OriginalFn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t *, int&, int&);
		getvfunc<OriginalFn>(this, 75)(this, font, text, wide, tall);
	}
};

class CEntList
{
public:
	CBaseEntity* GetClientEntity( int entnum )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 3 )( this, entnum );
	}
	CBaseEntity* GetClientEntityFromHandle( int hEnt )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 4 )( this, hEnt );
	}
	int GetHighestEntityIndex(void)
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 6 )( this );
	}
};

enum playercontrols
{
	IN_ATTACK = (1 << 0),
	IN_JUMP	= (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),	// Used by client.dll for when scoreboard is held down
	IN_SPEED = (1 << 17),	// Player is holding the speed key
	IN_WALK = (1 << 18),	// Player holding walk key
	IN_ZOOM	= (1 << 19),	// Zoom key for HUD zoom
	IN_WEAPON1 = (1 << 20),	// weapon defines these bits
	IN_WEAPON2 = (1 << 21),	// weapon defines these bits
	IN_BULLRUSH = (1 << 22),
};

enum tf_cond 
{ 
    TFCond_Slowed = (1 << 0), //Toggled when a player is slowed down. 
    TFCond_Zoomed = (1 << 1), //Toggled when a player is zoomed. 
    TFCond_Disguising = (1 << 2), //Toggled when a Spy is disguising.  
    TFCond_Disguised = (1 << 3), //Toggled when a Spy is disguised. 
    TFCond_Cloaked = (1 << 4), //Toggled when a Spy is invisible. 
    TFCond_Ubercharged = (1 << 5), //Toggled when a player is ÜberCharged. 
    TFCond_TeleportedGlow = (1 << 6), //Toggled when someone leaves a teleporter and has glow beneath their feet. 
    TFCond_Taunting = (1 << 7), //Toggled when a player is taunting. 
    TFCond_UberchargeFading = (1 << 8), //Toggled when the ÜberCharge is fading. 
    TFCond_CloakFlicker = (1 << 9), //Toggled when a Spy is visible during cloak. 
    TFCond_Teleporting = (1 << 10), //Only activates for a brief second when the player is being teleported; not very useful. 
    TFCond_Kritzkrieged = (1 << 11), //Toggled when a player is being crit buffed by the KritzKrieg. 
    TFCond_TmpDamageBonus = (1 << 12), //Unknown what this is for. Name taken from the AlliedModders SDK. 
    TFCond_DeadRingered = (1 << 13), //Toggled when a player is taking reduced damage from the Deadringer. 
    TFCond_Bonked = (1 << 14), //Toggled when a player is under the effects of The Bonk! Atomic Punch. 
    TFCond_Stunned = (1 << 15), //Toggled when a player's speed is reduced from airblast or a Sandman ball. 
    TFCond_Buffed = (1 << 16), //Toggled when a player is within range of an activated Buff Banner. 
    TFCond_Charging = (1 << 17), //Toggled when a Demoman charges with the shield. 
    TFCond_DemoBuff = (1 << 18), //Toggled when a Demoman has heads from the Eyelander. 
    TFCond_CritCola = (1 << 19), //Toggled when the player is under the effect of The Crit-a-Cola. 
    TFCond_InHealRadius = (1 << 20), //Unused condition, name taken from AlliedModders SDK. 
    TFCond_Healing = (1 << 21), //Toggled when someone is being healed by a medic or a dispenser. 
    TFCond_OnFire = (1 << 22), //Toggled when a player is on fire. 
    TFCond_Overhealed = (1 << 23), //Toggled when a player has >100% health. 
    TFCond_Jarated = (1 << 24), //Toggled when a player is hit with a Sniper's Jarate. 
    TFCond_Bleeding = (1 << 25), //Toggled when a player is taking bleeding damage. 
    TFCond_DefenseBuffed = (1 << 26), //Toggled when a player is within range of an activated Battalion's Backup. 
    TFCond_Milked = (1 << 27), //Player was hit with a jar of Mad Milk. 
    TFCond_MegaHeal = (1 << 28), //Player is under the effect of Quick-Fix charge. 
    TFCond_RegenBuffed = (1 << 29), //Toggled when a player is within a Concheror's range. 
    TFCond_MarkedForDeath = (1 << 30), //Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated. 
	TFCond_NoHealingDamageBuff = (1 << 31), //Unknown what this is used for.

    TFCondEx_SpeedBuffAlly = (1 << 0), //Toggled when a player gets hit with the disciplinary action. 
    TFCondEx_HalloweenCritCandy = (1 << 1), //Only for Scream Fortress event maps that drop crit candy. 
	TFCondEx_CritCanteen = (1 << 2), //Player is getting a crit boost from a MVM canteen.
	TFCondEx_CritDemoCharge = (1 << 3), //From demo's shield
	TFCondEx_CritHype = (1 << 4), //Soda Popper crits. 
    TFCondEx_CritOnFirstBlood = (1 << 5), //Arena first blood crit buff. 
    TFCondEx_CritOnWin = (1 << 6), //End of round crits. 
    TFCondEx_CritOnFlagCapture = (1 << 7), //CTF intelligence capture crits. 
    TFCondEx_CritOnKill = (1 << 8), //Unknown what this is for. 
    TFCondEx_RestrictToMelee = (1 << 9), //Unknown what this is for. 
	TFCondEx_DefenseBuffNoCritBlock = ( 1 << 10 ), //MvM Buff.
	TFCondEx_Reprogrammed = (1 << 11), //MvM Bot has been reprogrammed.
    TFCondEx_PyroCrits = (1 << 12), //Player is getting crits from the Mmmph charge. 
    TFCondEx_PyroHeal = (1 << 13), //Player is being healed from the Mmmph charge. 
	TFCondEx_FocusBuff = (1 << 14), //Player is getting a focus buff.
	TFCondEx_DisguisedRemoved = (1 << 15), //Disguised remove from a bot.
	TFCondEx_MarkedForDeathSilent = (1 << 16), //Player is under the effects of the Escape Plan/Equalizer or GRU.
	TFCondEx_DisguisedAsDispenser = (1 << 17), //Bot is disguised as dispenser.
	TFCondEx_Sapped = (1 << 18), //MvM bot is being sapped.
	TFCondEx_UberchargedHidden = (1 << 19), //MvM Related
	TFCondEx_UberchargedCanteen = (1 << 20), //Player is receiving ÜberCharge from a canteen.
	TFCondEx_HalloweenBombHead = (1 << 21), //Player has a bomb on their head from Merasmus.
	TFCondEx_HalloweenThriller = (1 << 22), //Players are forced to dance from Merasmus.
	TFCondEx_BulletCharge = (1 << 26), //Player is receiving 75% reduced damage from bullets.
	TFCondEx_ExplosiveCharge = (1 << 27), //Player is receiving 75% reduced damage from explosives.
	TFCondEx_FireCharge = (1 << 28), //Player is receiving 75% reduced damage from fire.
	TFCondEx_BulletResistance = (1 << 29), //Player is receiving 10% reduced damage from bullets.
	TFCondEx_ExplosiveResistance = (1 << 30), //Player is receiving 10% reduced damage from explosives.
	TFCondEx_FireResistance = (1 << 31), //Player is receiving 10% reduced damage from fire.

	TFCondEx2_Stealthed = (1 << 0),
	TFCondEx2_MedigunDebuff = (1 << 1),
	TFCondEx2_StealthedUserBuffFade = (1 << 2),
	TFCondEx2_BulletImmune = (1 << 3),
	TFCondEx2_BlastImmune = (1 << 4),
	TFCondEx2_FireImmune = (1 << 5),
	TFCondEx2_PreventDeath = (1 << 6),
	TFCondEx2_MVMBotRadiowave = (1 << 7),
	TFCondEx2_HalloweenSpeedBoost = (1 << 8), //Wheel has granted player speed boost.
	TFCondEx2_HalloweenQuickHeal = (1 << 9), //Wheel has granted player quick heal.
	TFCondEx2_HalloweenGiant = (1 << 10), //Wheel has granted player giant mode.
	TFCondEx2_HalloweenTiny = (1 << 11), //Wheel has granted player tiny mode.
	TFCondEx2_HalloweenInHell = (1 << 12), //Wheel has granted player in hell mode.
	TFCondEx2_HalloweenGhostMode = (1 << 13), //Wheel has granted player ghost mode.
	TFCondEx2_Parachute = (1 << 16), //Player has deployed the BASE Jumper.
	TFCondEx2_BlastJumping = (1 << 17), //Player has sticky or rocket jumped.

    TFCond_MiniCrits = ( TFCond_Buffed | TFCond_CritCola ),
    TFCond_IgnoreStates = ( TFCond_Ubercharged | TFCond_Bonked ), 
    TFCondEx_IgnoreStates = ( TFCondEx_PyroHeal ) 
};

enum tf_classes
{
	TF2_Scout = 1,
	TF2_Soldier = 3,
	TF2_Pyro = 7,
	TF2_Demoman = 4,
	TF2_Heavy = 6,
	TF2_Engineer = 9,
	TF2_Medic = 5,
	TF2_Sniper = 2,
	TF2_Spy = 8,
};

class ClientModeShared
{
public:
	bool IsChatPanelOutOfFocus(void)
	{
		typedef PVOID(__thiscall* OriginalFn)(PVOID);
		PVOID CHudChat = getvfunc<OriginalFn>(this, 19)(this);
		if (CHudChat)
		{
			return *(PFLOAT)((DWORD)CHudChat + 0xFC) == 0;
		}
		return false;
	}
};

#include "CTrace.hpp"

class CLag
{
public:
	float timestamp;
	int m_bChokedPackets;
	Vector m_bViewAngles;
};

#include "Prediction.h"
class CPlayerInformation
{
public:
	CBaseEntity * hEntity;
	bool scoped;
};

enum MatVarFlags
{
	IGNOREZ = 0x8000,
	FLAT = 0x1000,
	NDRAW = 0x0004,
};

#include <public/materialsystem/imaterial.h>
class CMaterial : public IMaterial
{
	const char* GetName()
	{
		typedef const char*(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 0)(this);
	}
	void SetMatVarFlag(MatVarFlags flags, bool ok)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, MatVarFlags, bool);
		return getvfunc<OriginalFn>(this, 29)(this, flags, ok);
	}
};

#include <ivrenderview.h>
class CRenderView : public IVRenderView
{
};

class IMesh;
class IVertexBuffer;
class IIndexBuffer;
struct MaterialSystem_Config_t;
class VMatrix;
class matrix3x4_t;
class ITexture;
struct MaterialSystemHWID_t;
class KeyValues;
class IShader;
class IVertexTexture;
class IMorph;
class IMatRenderContext;
class ICallQueue;
struct MorphWeight_t;
class IFileList;
struct VertexStreamSpec_t;
struct ShaderStencilState_t;
struct MeshInstanceData_t;
class IClientMaterialSystem;
class CPaintMaterial;
class IPaintMapDataManager;
class IPaintMapTextureManager;
class GPUMemoryStats;
struct AspectRatioInfo_t;
struct CascadedShadowMappingState_t;

class IMaterialProxyFactory;
class ITexture;
class IMaterialSystemHardwareConfig;
class CShadowMgr;

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};


typedef int ModelInstanceHandle_t;

struct ModelRenderInfo_t
{
	Vector origin;
	QAngle angles;
	PVOID *pRenderable;
	const DWORD *pModel;
	const matrix3x4_t *pModelToWorld;
	const matrix3x4_t *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

class CModelRender
{
public:
	void ForcedMaterialOverride(IMaterial *mat, OverrideType_t type)
	{
		typedef void(__thiscall * OriginalFn)(PVOID, IMaterial *, OverrideType_t);
		return getvfunc<OriginalFn>(this, 1)(this, mat, type);
	}

	void DrawModelExecute(void *state, ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		typedef void(__thiscall * OriginalFn)(PVOID, void *, ModelRenderInfo_t &, matrix3x4_t *);
		return getvfunc<OriginalFn>(this, 19)(this, state, pInfo, pCustomBoneToWorld);
	}

	void SuppressEngineLighting(bool t)
	{
		typedef void(__thiscall * OriginalFn)(PVOID, bool);
		return getvfunc<OriginalFn>(this, 22)(this, t);
	}
};

#include <basehandle.h>

class CMaterialSystem
{
public:
#ifdef _HL2DM_MP_v1
	IMaterial * FindMaterial(char const *pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial *(__thiscall * OriginalFn)(PVOID, const char *, const char *, bool, const char *);
		return getvfunc<OriginalFn>(this, 71)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	ITexture *FindTexture(char const *pTextureName, const char *pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0)
	{
		typedef ITexture *(__thiscall * OriginalFn)(PVOID, const char *, const char *, bool, int);
		return getvfunc<OriginalFn>(this, 79)(this, pTextureName, pTextureGroupName, complain, nAdditionalCreationFlags);
	}

	IMaterial* GetMaterial(int i)
	{
		typedef IMaterial*(__thiscall* OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 76)(this, i);
	}

	unsigned short InvalidMaterial()
	{
		typedef signed int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 75)(this);
	}

	ITexture *CreateNamedRenderTargetTextureEx(const char *                rtName,
		int                         w,
		int                         h,
		RenderTargetSizeMode_t      sizeMode, // Controls how size is generated (and regenerated on video mode change).
		ImageFormat                 format,
		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
		unsigned int                textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
		unsigned int                renderTargetFlags = 0)
	{
		typedef ITexture *(__thiscall * OriginalFn)(PVOID, char const *, int, int, RenderTargetSizeMode_t, ImageFormat, MaterialRenderTargetDepth_t, unsigned int, unsigned int);

		return getvfunc<OriginalFn>(this, 85)(this, rtName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags);
	}

	IMatRenderContext *GetRenderContext()
	{
		typedef IMatRenderContext *(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 98)(this);
	}

	IMaterial *CreateMaterial_Internal(const char *pMaterialName, KeyValues *pKeyValues)
	{
		typedef IMaterial *(__thiscall * OriginalFn)(PVOID, const char *, KeyValues *);
		auto *material = getvfunc<OriginalFn>(this, 72)(this, pMaterialName, pKeyValues);

		material->IncrementReferenceCount();

		return material;
	}

	IMaterial *CreateMaterial(const char *pMaterialName, bool flat, bool ignorez, bool wireframe)
	{
		typedef IMaterial *(__thiscall * OriginalFn)(PVOID, const char *, KeyValues *);

		std::string type = (flat) ? "UnlitGeneric" : "VertexLitGeneric";

		std::string matdata = "\"" + type + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$envmap\"  \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"1\"\n\t\"$nocull\"  \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\"  \"0\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n}\n";

		KeyValues *pKeyValues = new KeyValues(type.c_str());
		// U::InitKeyValues(pKeyValues, type.c_str());
		pKeyValues->LoadFromBuffer(pMaterialName, matdata.c_str());

		auto *material = getvfunc<OriginalFn>(this, 70)(this, pMaterialName, pKeyValues);

		material->IncrementReferenceCount();

		return material;
	}
#else
	IMaterial * FindMaterial(char const *pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial *(__thiscall * OriginalFn)(PVOID, const char *, const char *, bool, const char *);
		return getvfunc<OriginalFn>(this, 73)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	ITexture *FindTexture(char const *pTextureName, const char *pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0)
	{
		typedef ITexture *(__thiscall * OriginalFn)(PVOID, const char *, const char *, bool, int);
		return getvfunc<OriginalFn>(this, 81)(this, pTextureName, pTextureGroupName, complain, nAdditionalCreationFlags);
	}

	IMaterial* GetMaterial(int i)
	{
		typedef IMaterial*(__thiscall* OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 78)(this, i);
	}

	unsigned short InvalidMaterial()
	{
		typedef signed int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 77)(this);
	}

	ITexture *CreateNamedRenderTargetTextureEx(const char *                rtName,
		int                         w,
		int                         h,
		RenderTargetSizeMode_t      sizeMode, // Controls how size is generated (and regenerated on video mode change).
		ImageFormat                 format,
		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
		unsigned int                textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
		unsigned int                renderTargetFlags = 0)
	{
		typedef ITexture *(__thiscall * OriginalFn)(PVOID, char const *, int, int, RenderTargetSizeMode_t, ImageFormat, MaterialRenderTargetDepth_t, unsigned int, unsigned int);

		return getvfunc<OriginalFn>(this, 87)(this, rtName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags);
	}

	IMatRenderContext *GetRenderContext()
	{
		typedef IMatRenderContext *(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 100)(this);
	}

	IMaterial *CreateMaterial_Internal(const char *pMaterialName, KeyValues *pKeyValues)
	{
		typedef IMaterial *(__thiscall * OriginalFn)(PVOID, const char *, KeyValues *);
		auto *material = getvfunc<OriginalFn>(this, 72)(this, pMaterialName, pKeyValues);

		material->IncrementReferenceCount();

		return material;
	}

	IMaterial *CreateMaterial(const char *pMaterialName, bool flat, bool ignorez, bool wireframe)
	{
		typedef IMaterial *(__thiscall * OriginalFn)(PVOID, const char *, KeyValues *);

		std::string type = (flat) ? "UnlitGeneric" : "VertexLitGeneric";

		std::string matdata = "\"" + type + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$envmap\"  \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"1\"\n\t\"$nocull\"  \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\"  \"0\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n}\n";

		KeyValues *pKeyValues = new KeyValues(type.c_str());
		// U::InitKeyValues(pKeyValues, type.c_str());
		pKeyValues->LoadFromBuffer(pMaterialName, matdata.c_str());

		auto *material = getvfunc<OriginalFn>(this, 72)(this, pMaterialName, pKeyValues);

		material->IncrementReferenceCount();

		return material;
	}
#endif
};

#include <game/shared/igamemovement.h>

class CTraceData001
{
public:
	float dist_till_hit;
	QAngle original;
};

class CGameMovement
{
public:
	virtual ~CGameMovement(void)
	{
	}

	// Process the current movement command
	virtual void ProcessMovement(CBaseEntity *pPlayer, CMoveData *pMove) = 0;
	virtual void StartTrackPredictionErrors(CBaseEntity *pPlayer) = 0;
	virtual void FinishTrackPredictionErrors(CBaseEntity *pPlayer) = 0;
	virtual void DiffPrint(char const *fmt, ...) = 0;

	// Allows other parts of the engine to find out the normal and ducked player bbox sizes
	virtual Vector GetPlayerMins(bool ducked) const = 0;
	virtual Vector GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector GetPlayerViewOffset(bool ducked) const = 0;
};

class CPrediction
{
public:
	void SetupMove(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *helper, CMoveData *move)
	{
		return getvfunc<void(__thiscall *)(void *, CBaseEntity *, CUserCmd *, IMoveHelper *, CMoveData *)>(this, 18)(this, player, ucmd, helper, move);
	}
	void FinishMove(CBaseEntity *player, CUserCmd *ucmd, CMoveData *move)
	{
		return getvfunc<void(__thiscall *)(void *, CBaseEntity *, CUserCmd *, CMoveData *)>(this, 19)(this, player, ucmd, move);
	}
};

#include "EngineServer.hpp"
#include "cstate.hpp"
#include "Interfaces.hpp"
extern CPlayerVariables gPlayerVars;
extern COffsets gOffsets;
extern CVars gOpts;

typedef const char* c_char;

#include "bsp.hpp"

class c_baseplayer;

#ifdef _HL2DM_MP_v1

///
/// HL2DM PHYSGUN RELATED
/// NOT DONE / APPROVED!
///

class IWeapon {
public:
};

class CHoldEntity {
public:
	inline bool IsBeingHeld() {
		return *(DWORD*)(this + 0x0714) >= 0x1; //Works;
	}
};

class IBasePhysCannon
{
public:
	inline int GetClip1() {
		return *(int*)(this + 0x08BC); //Just use the netvar???;
	}

	CBaseEntity* GetHeldEntity();
};

#endif

class c_baserecord {
public:
	inline c_baserecord() {
		m_Curtime = -1;
		m_Tickbase = -1;
		m_bIsUsed = false;

		m_AbsOrigin = Vector();
		m_OutOrigin = Vector();
	}

	//values;
	float m_Curtime;
	float m_Tickbase;
	bool m_bIsUsed;
	Vector m_AbsOrigin;
	Vector m_OutOrigin;
	INetChannel* m_NetChannel;
	void* entity;
	//....;
};