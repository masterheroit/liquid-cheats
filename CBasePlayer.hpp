#pragma once

#ifndef __BASEPLAYER_HPP

#define __BASEPLAYER_HPP

#include "SDK.h"
#include "Netvars.hpp"
#include "Maths.hpp"

enum falltype_t
{
	FALL_NONE,
	FALL_UP,
	FALL_DOWN,
};

class CNetVars
{
public:
	DWORD m_lifeState; //0xA5;

	DWORD m_playerCnd; //0x0;

	DWORD m_iHealth; //0x0;

	DWORD m_iFlags; //0x0;

	DWORD m_iTeamNum; //0x0;

	DWORD m_vViewOffset; //0x0;

	DWORD m_dwDeadFlag;	//0x0;

	DWORD m_iFOV; //0x0;

	DWORD m_iPlayerCond; //0x0;

	DWORD m_localEyeAnglesPitch; //0x0;
	
	DWORD m_localEyeAnglesYaw; //0x0;

	DWORD m_netEyeAnglesPitch; //0x0;

	DWORD m_netEyeAnglesYaw; //0x0;

	DWORD m_pPlayerClass; //0x0;

	DWORD m_vVecVelocity; //0x0;

	DWORD m_iItemDefIndex; //0x0;

	DWORD m_flLastFireTime; //0x0;

	DWORD m_iClip1; //0x0;

	DWORD m_iTickBase; //0x0;

	DWORD m_nSequence; //0x0;

	DWORD m_flMediguCharge; //0x0;

	DWORD m_collMins; //0x0;

	DWORD m_collMaxs; //0x0;

	DWORD m_collideable; //0x0;

	void initialize(); //0x10;
};

extern CNetVars g_Netvars;

enum source_hitboxes
{
	HITBOX_HEAD = 0, //0x0;
	HITBOX_BODY = 1, //0x1;
};

class CBaseSentry : public CBaseEntity
{
public:
	int Team()
	{
		return *(int*)(this + g_Netvars.m_iTeamNum);
	}
};

class CBaseWeapon : public CBaseEntity
{
public:
	float get_charge_time()
	{
		return *(float*)(this + 0x570);
	}
	float flLastFire()
	{
		return *(float*)(this + g_Netvars.m_flLastFireTime);
	}
	float get_next_attack()
	{
		return *(float*)(this + 0xA64);
	}
	int GetItemIndex()
	{
		return *(int*)(this + g_Netvars.m_iItemDefIndex);
	}
	int GetClip()
	{
		return *(int*)(this + g_Netvars.m_iClip1);
	}
#ifdef _HL2DM_MP_v1
	inline bool IsReloading() {
		return *(DWORD*)(this + 0x0884) >= 0x1; //Works, this one is a static variable though.
	}
#endif
};

class c_baseplayer : public CBaseEntity
{
public:
	/*
	(3D3638B0 + 1028) NETVAR	dynamic //OK; (done via dynamic-netvars aleady.)
	(3D3638B0 + 102C) NETVAR	dynamic //OK; (done via dynamic-netvars already.)
	(3D3638B0 + 0x164) unk000	dynamic //OK; (done via rc offset + base.)
	*/
	int iTickBase()
	{
		return *(int*)(this + g_Netvars.m_iTickBase);
	}
	int DrawModel(int flags) {
		const PVOID pRenderable = (const PVOID)(this + 0x4);
		typedef int(__thiscall * OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(pRenderable, 10)(pRenderable, flags);
	}
	void UpdateAnimations()
	{
		const void *pAnimating = (const void *)(this + 0x4);
		return getvfunc<void(__thiscall *)(const void *)>(this, 191)(this);
	}
	float GetMedicChargeLevel()
	{
		float a;

		this->CreatePtr<float>(g_Netvars.m_flMediguCharge, &a);

		return a;
	}
	void SetEyeAngles(QAngle angle)
	{
		auto pitch		= reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + g_Netvars.m_netEyeAnglesPitch);
		auto yaw		= reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + g_Netvars.m_netEyeAnglesYaw);

		*pitch = angle.x;
		*yaw = angle.y;
	}
	falltype_t GetVelType()
	{
		auto __unk001 = *(__int32*)(this + 0x164);

		if (__unk001 == 0)
			return FALL_NONE;
		else if (__unk001 < 0)
			return FALL_UP;
		else if (__unk001 > 0)
			return FALL_DOWN;
	}
	int GetSequenceNr()
	{
		int a;

		this->CreatePtr<int>(g_Netvars.m_nSequence, &a);

		return a;
	}
	Vector GetCollMins() {
		return *(Vector*)(this + g_Netvars.m_collMins);
	}
	Vector GetCollMaxs() {
		return *(Vector*)(this + g_Netvars.m_collMaxs);
	}
	QAngle GetEyeAngles()
	{
		QAngle ptr;

		float a;
		float b;
		this->CreatePtr<float>(g_Netvars.m_netEyeAnglesPitch, &a);
		this->CreatePtr<float>(g_Netvars.m_netEyeAnglesYaw, &b);

		return (QAngle)(QAngle{ a, b, 0.f });
	}
	CBaseWeapon* get_weapon()
	{
#ifndef _HL2DM_MP_v1
		auto pWpnPtr = *(DWORD*)(this + 0xDB8);
#else
		auto pWpnPtr = *(DWORD*)(this + 0xDF8);
#endif

		return (CBaseWeapon*)I::EntList->GetClientEntityFromHandle(pWpnPtr);
	}
	bool set_cond(int conds)
	{
		auto ptr = reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + g_Netvars.m_iPlayerCond);

		*ptr = conds;

		return true;
	}
	int GetConds()
	{
		int ptr;

		this->CreatePtr<int>(g_Netvars.m_iPlayerCond, &ptr);

		return ptr;
	}

	int GetClassById()
	{
		int ptr;

		this->CreatePtr<int>(g_Netvars.m_pPlayerClass, &ptr);

		return ptr;
	}

	const char* GetClassByName()
	{
		int ptr;

		auto ret = "None";

		this->CreatePtr<int>(g_Netvars.m_pPlayerClass, &ptr);

		switch (ptr)
		{
		case TF2_Sniper:
			ret = "Sniper";
			break;
		case TF2_Demoman:
			ret = "Demoman";
			break;
		case TF2_Engineer:
			ret = "Engineer";
			break;
		case TF2_Heavy:
			ret = "Heavy";
			break;
		case TF2_Medic:
			ret = "Medic";
			break;
		case TF2_Scout:
			ret = "Scout";
			break;
		case TF2_Soldier:
			ret = "Soldier";
			break;
		case TF2_Spy:
			ret = "Spy";
			break;
		case TF2_Pyro:
			ret = "Pyro";
			break;
		default:
			ret = "None";
		}

		return ret;
	}
	Vector VecVelocity()
	{
		Vector ptr;

		this->CreatePtr<Vector>(g_Netvars.m_vVecVelocity, &ptr);

		return ptr;
	}
	template <class T>
	void CreatePtr(DWORD offs, T* t)
	{
		*t = *(T*)(this + offs);
	}
	template <class T>
	T* CreatePtr_t(DWORD offs)
	{
		return reinterpret_cast<T*>(reinterpret_cast<DWORD>(this) + offs);
	}
	Vector GetVecHitboxPos(int hitbox)
	{
		auto ret = Vector();

		DWORD *model = this->GetModel();

		if (!model)
			return ret;

		studiohdr_t *hdr = I::MdlInfo->GetStudiomodel((model_t*)model);

		if (!hdr)
			return ret;

		matrix3x4 matrix[128];

		if (!this->SetupBones(matrix, 128, 0x100, 0))
			return ret;

		int HitboxSetIndex = *(int *)((DWORD)hdr + 0xB0);

		if (!HitboxSetIndex)
			return ret;

		mstudiohitboxset_t *pSet = (mstudiohitboxset_t *)(((PBYTE)hdr) + HitboxSetIndex);

		mstudiobbox_t* box = pSet->pHitbox(hitbox);

		if (!box)
			return ret;

		Vector vCenter = (box->bbmin + box->bbmax) * 0.5f;

		Vector vHitbox;

		Maths::VectorTransform(vCenter, matrix[box->bone], vHitbox);

		return vHitbox;
	}
	template < class T >
	T GetVecHitboxPos(int hitbox, int rd)
	{
		auto ret = Vector();

		DWORD *model = this->GetModel();

		if (!model) 			return ret;

		studiohdr_t *hdr = I::MdlInfo->GetStudiomodel(model);

		if (!hdr)			return ret;

		matrix3x4 matrix[128];

		if (!this->SetupBones(matrix, 128, 0x100, 0)) 			return ret;

		int HitboxSetIndex = *(int *)((DWORD)hdr + 0xB0);

		if (!HitboxSetIndex) 			return ret;

		mstudiohitboxset_t *pSet = (mstudiohitboxset_t *)(((PBYTE)hdr) + HitboxSetIndex);

		mstudiobbox_t* box = pSet->pHitbox(hitbox);

		if (!box)			return ret;

		Vector vCenter = (box->bbmin + box->bbmax) * rd;

		Vector vHitbox;

		Maths::VectorTransform(vCenter, matrix[box->bone], vHitbox);

		return (T&)vHitbox;
	}
	bool Alive()
	{
		byte ptr;

		this->CreatePtr<byte>(g_Netvars.m_lifeState, &ptr);
		
		return (bool)(ptr == LIFE_ALIVE);
	}
	Vector EyePosition()
	{
		Vector ptr;

		return *(Vector*)(this + g_Netvars.m_vViewOffset) + this->GetAbsOrigin();
	}
	const char* TranslateClassToFile()
	{
		switch (this->GetClassById())
		{
		case TF2_Sniper: return "C:\\icons\\radar\\sniper.png";
			break;
		case TF2_Scout: return "C:\\icons\\radar\\scout.png";
			break;
		case TF2_Soldier: return "C:\\icons\\radar\\soldier.png";
			break;
		case TF2_Spy: return "C:\\icons\\radar\\spy.png";
			break;
		case TF2_Pyro: return "C:\\icons\\radar\\pyro.png";
			break;
		case TF2_Engineer: return "C:\\icons\\radar\\engineer.png";
			break;
		case TF2_Medic: return "C:\\icons\\radar\\medic.png";
			break;
		case TF2_Demoman: return "C:\\icons\\radar\\demoman.png";
			break;
		case TF2_Heavy: return "C:\\icons\\radar\\heavy.png";
			break;
		default:
			return "C:\\icons\\radar\\none.png";
			break;
		}
	}
	int Team()
	{
		int ptr;

		this->CreatePtr<int>(g_Netvars.m_iTeamNum, &ptr);

		return ptr;
	}
	int Health()
	{
		int ptr;

		this->CreatePtr<int>(g_Netvars.m_iHealth, &ptr);

		return ptr;
	}
	int iFlags()
	{
		int ptr;

		this->CreatePtr<int>(g_Netvars.m_iFlags, &ptr);

		return ptr;
	}
};

//Further Definitions;
#define GetBasePlayer (c_baseplayer*)I::EntList->GetClientEntity
#endif