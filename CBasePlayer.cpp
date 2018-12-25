#include "CBasePlayer.hpp"

CNetVars g_Netvars;

void CNetVars::initialize()
{
	netvar_tree tree;

	//Globals;
	this->m_lifeState = tree.get_offset("DT_BasePlayer", "m_lifeState");
	this->m_iHealth = tree.get_offset("DT_BasePlayer", "m_iHealth");
	this->m_iFlags = tree.get_offset("DT_BasePlayer", "m_fFlags");
	this->m_vViewOffset = tree.get_offset("DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
	this->m_iTickBase = tree.get_offset("DT_BasePlayer", "localdata", "m_nTickBase");
	this->m_iTeamNum = tree.get_offset("DT_BaseEntity", "m_iTeamNum");
	this->m_dwDeadFlag = tree.get_offset("DT_BasePlayer", "pl", "deadflag");
	this->m_iFOV = tree.get_offset("DT_BasePlayer", "m_iFOV");
	this->m_vVecVelocity = tree.get_offset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
	this->m_collMins = tree.get_offset("DT_BaseEntity", "m_Collision", "m_vecMins");
	this->m_collMaxs = tree.get_offset("DT_BaseEntity", "m_Collision", "m_vecMaxs");
	this->m_collideable = tree.get_offset("DT_BaseEntity", "m_Collision");

	//Game Depnd...;
	switch (!I::EngineServer ? I::Engine->GetAppId() : I::EngineServer->GetAppID())
	{
	case 440:
	{
		this->m_iPlayerCond = tree.get_offset("DT_TFPlayer", "m_Shared", "m_nPlayerCond");
		this->m_localEyeAnglesPitch = tree.get_offset("DT_TFPlayer", "tflocaldata", "m_angEyeAngles[0]");
		this->m_localEyeAnglesYaw = tree.get_offset("DT_TFPlayer", "tflocaldata", "m_angEyeAngles[1]");
		this->m_netEyeAnglesPitch = tree.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
		this->m_netEyeAnglesYaw = tree.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[1]");
		this->m_iItemDefIndex = tree.get_offset("DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		this->m_pPlayerClass = tree.get_offset("DT_TFPlayer", "m_PlayerClass", "m_iClass");
		this->m_flLastFireTime = tree.get_offset("DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime");
		this->m_iClip1 = tree.get_offset("DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip1");
		this->m_playerCnd = tree.get_offset("DT_TFPlayer", "m_Shared", "m_nPlayerCond");
		this->m_nSequence = tree.get_offset("DT_BaseAnimating", "m_nSequence");
		this->m_flMediguCharge = tree.get_offset("DT_WeaponMedigun", "NonLocalTFWeaponMedigunData", "m_flChargeLevel");
	}
		break;
	case 240:
	{
		this->m_localEyeAnglesPitch = tree.get_offset("DT_CSPlayer", "m_angEyeAngles[0]");
		this->m_localEyeAnglesYaw = tree.get_offset("DT_CSPlayer", "m_angEyeAngles[1]");
		this->m_netEyeAnglesPitch = tree.get_offset("DT_CSPlayer", "m_angEyeAngles[0]");
		this->m_netEyeAnglesYaw = tree.get_offset("DT_CSPlayer", "m_angEyeAngles[1]");
		break;
	}
	case 320:
		this->m_netEyeAnglesPitch = 0x152C;
		this->m_netEyeAnglesYaw = 0x1530;
	}
}