#include "Aim.hpp"

static bool willshoot = false;

CAim g_Aim;

Vector GetOptimalBody(c_baseplayer* local, c_baseplayer* entity)
{
	hitbox body[9] = {
		hitbox::hip_L,
		hitbox::hip_R,
		hitbox::spine_0,
		hitbox::spine_1,
		hitbox::hand_L,
		hitbox::hand_R,
		hitbox::spine_2,
		hitbox::spine_3,
		hitbox::pelvis,
	};

	for each(auto obj in body)
	{
		if (Maths::IsVisible(local, entity, g_pPredData008.new_eyepos, entity->GetVecHitboxPos(obj)))
			return entity->GetVecHitboxPos(obj);
	}

	return Vector(0, 0, 0);
}

BOOL CanShoot(c_baseplayer* local)
{
	CBaseWeapon *local_weapon = local->get_weapon();

	if (local_weapon == nullptr)
		return false;

	static float lastFire = 0, nextAttack = 0;
	static CBaseEntity* old_weapon;

	if (lastFire != local_weapon->flLastFire() || local_weapon != old_weapon)
	{
		lastFire = local_weapon->flLastFire();
		nextAttack = local_weapon->get_next_attack();
	}

	if (local_weapon->GetClip() == 0)
		return false;

	old_weapon = local_weapon;

	if ((nextAttack <= (float)(local->iTickBase()) * I::Globals->interval_per_tick))
	{
		return true;
	}

	willshoot = true;
}

bool Predict(Vector vel, Vector &vHitpos, c_baseplayer* entity)
{
	if (!entity)  return false;
}

int GetBestPlayer(CUserCmd* cmd, c_baseplayer* local, bool isproj)
{
	auto ret = -1;

	float flBest = 999999999.f;

#ifndef _HL2DM_MP_v1
	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++)
	{
#else
	for (int i = 0; i <= I::Engine->GetMaxClients(); i++)
	{
#endif
		auto entity = GetBasePlayer(i);

#ifndef _HL2DM_MP_v1
		if (!local || !entity || entity->IsDormant() || entity->Alive() != true || entity->IsPlayer() != true)
			continue;
#else
		if (!local || !entity || entity->IsDormant() || entity->Alive() != true)
			continue;
#endif

		auto wpn = local->get_weapon();

		if (!wpn)
			continue;

		if (!gOpts.aimbot_dm_attack_teammates) {
			if (local->Team() == entity->Team())
				continue;
		}

		Vector vAimPos;

		if (gOpts.aimbot_hitbox == 1)		//Head Only;
			vAimPos = entity->GetVecHitboxPos(HITBOX_HEAD);
		else if (gOpts.aimbot_hitbox == 2)	//Adjust for Weapon(s);
			vAimPos = entity->GetVecHitboxPos(isproj ? HITBOX_BODY : HITBOX_HEAD);
		else if (gOpts.aimbot_hitbox == 3)	//Body Only;
			vAimPos = entity->GetVecHitboxPos(HITBOX_BODY);
		else if (gOpts.aimbot_hitbox == 4)	//Dynamic Body;
			vAimPos = GetOptimalBody(local, entity);

		if (entity->GetConds() & TFCond_Ubercharged) continue;

		//Check if entity iciy valid, entity is current index, + (and) tick isnt the current one;

		if (gOpts.accuracy_backtrack && Maths::IsVisible(local, entity, g_pPredData008.new_eyepos, vAimPos))
		{
			vAimPos = g_Btdata->data->hitpos[i]->ptr;
		}

		if(!(Maths::IsVisible(local, entity, g_pPredData008.new_eyepos, vAimPos)))
			continue;

		if (gOpts.aimbot_targeting_mode == 0)
		{
			auto vHitbox = entity->GetVecHitboxPos(HITBOX_HEAD);

			auto vEyePos = g_pPredData008.new_eyepos;

			float fDistance = (float)(vHitbox - vEyePos).Length();

			if (fDistance < 99999999.f)
			{
				ret = i;
			}
		}
		else if (gOpts.aimbot_targeting_mode == 1)
		{
			auto vHitbox = entity->GetVecHitboxPos(HITBOX_HEAD);

			auto vEyePos = g_pPredData008.new_eyepos;

			float flFOV = Maths::GetFOV(cmd->viewangles, vEyePos, vHitbox);
			float distance = Maths::flGetDistance(vHitbox, g_pPredData008.new_eyepos);

			if (distance < flBest)
			{
				if (flFOV < flBest && flFOV < gOpts.aimbot_max_fov)
				{
					//flDistToBest = flDistToTarget;
					flBest = flFOV;
					ret = i;
				}
			}
		}
	}

	return ret;
}

BOOL IsHealWeapon(int i)
{
	if (i == WPN_Vaccinator || i == WPN_Medigun)
		return TRUE;

	return FALSE;
}

bool ReadyToAim()
{
	bool ret = false;

	switch (gOpts.aimbot_aim_key) {
	case CKC_NONE:
		ret = true;
		break;
	case CKC_LEFT:
		ret = GetAsyncKeyState(VK_LBUTTON);
		break;
	case CKC_RIGHT:
		ret = GetAsyncKeyState(VK_RBUTTON);
		break;
	case CKC_SHIFT:
		ret = GetAsyncKeyState(VK_SHIFT);
		break;
	default:
		break;
	}

	return ret;
}

enum C_PredictionModes_t
{
	PREDICT_NONE = 000, //Don't predict the entity, at all!!!!
	PREDICT_NORAY = 001, //Don't predict the entity, at all, but peform some safety checks because the current position isn't aviable for prediction!!!!
	PREDICT_CPOSI = 100, //Predict our Current Prediction Position, this is gud!!!!
};

C_PredictionModes_t GetPredMode(c_baseplayer* entity, Vector vHitpos)
{
	CGameTrace trace;
	Ray_t ray;
	CTraceFilter filter;

	Vector vaPredAngs;

	filter.pSkip = entity;

	AngleVectors((QAngle&)vHitpos, &vaPredAngs);

	vaPredAngs *= 100;

	ray.Init(vHitpos, vaPredAngs);

	I::Trace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	if (trace.fraction < 0.97)
		return PREDICT_NORAY;
	else if (trace.fraction > 0.97)
		return PREDICT_CPOSI;
	else
		return PREDICT_NONE;
}

#include "projectiles.hpp"

void CAim::run(CUserCmd* cmd)
{
	if (!cmd)
		return;

	auto fmov	 =		0.f;
	auto smov	 =		0.f;
	auto vmov	 =		Vector(0.f, 0.f, 0.f);
	auto isproj	 =		false;
	auto pspeed	 =		0.0f;
	auto iTarget =		GetBestPlayer(cmd, GetBasePlayer(me), isproj);
	auto pTarget =		GetBasePlayer(iTarget);
	auto local	 =		GetBasePlayer(me);

	Maths::BackupSf(cmd, fmov, smov, vmov);

	if (!cmd || !pTarget || iTarget == -1 || ReadyToAim() != true || !gOpts.aimbot_active || CanShoot(local) != true)
		return;

	int weaponId = 0;

	if (!local->get_weapon())
		return;

	weaponId = local->get_weapon()->GetItemIndex();

	switch (weaponId)
	{
	case WPN_DirectHit:
		pspeed = 1980.0f;
	case WPN_RocketLauncher || WPN_BlackBox || WPN_Original || WPN_RocketJump || WPN_BeggersBazooka:
		pspeed = 1100.0f;
	}

	if (weaponId == WPN_DirectHit || weaponId == WPN_RocketLauncher)
		isproj = true;

	Vector vEyePos = g_pPredData008.new_eyepos;

	Vector vAimPos;

	if (gOpts.aimbot_hitbox < 0 || gOpts.aimbot_hitbox > 4)
		return;

	if (gOpts.aimbot_hitbox == 1)		//Head Only;
		vAimPos = pTarget->GetVecHitboxPos(HITBOX_HEAD);
	else if (gOpts.aimbot_hitbox == 2)	//Adjust for Weapon(s);
		vAimPos = pTarget->GetVecHitboxPos(isproj ? HITBOX_BODY : HITBOX_HEAD);
	else if (gOpts.aimbot_hitbox == 3)	//Body Only;
		vAimPos = pTarget->GetVecHitboxPos(HITBOX_BODY);
	else if (gOpts.aimbot_hitbox == 4)	//Dynamic Body;
		vAimPos = GetOptimalBody(local, pTarget);

	auto wpn = local->get_weapon();

	if (wpn) {
		auto m_Spread = *(int*)(wpn + 0x38);
		auto m_pRandom = int(RandomFloat(0, I::Globals->frametime) * m_Spread);
		auto m_Punch = int(*(int*)(wpn + 0xEFC));

		cmd->viewangles.x -= int(m_Spread * m_Punch);
		cmd->viewangles.z += m_pRandom;
	}

	Vector vFinalPos;

	if (gOpts.accuracy_backtrack)
	{
		vAimPos = g_Btdata->data->hitpos[iTarget]->ptr;
	}

	if (isproj)
	{
		if (!Projectiles::HelpersBase)
			Projectiles::HelpersBase = new C_BaseProjectileHelpers();

		Vector vecpos;

		Projectiles::HelpersBase->predict_vector(pTarget, local->get_weapon(), vAimPos, vecpos);

		auto pred = GetPredMode(pTarget, vecpos);

		if (pred == PREDICT_CPOSI)
			vAimPos = vecpos;
		else if (pred == PREDICT_NORAY)
		{
			auto pos = utils::trace_ray(local->EyePosition(), vAimPos, local).endpos;
			vAimPos = pos;
		}
		else
			vAimPos = vecpos;
	}

	Maths::VectorAngles((vAimPos - vEyePos), vFinalPos);
	Maths::Clamp(vFinalPos);

	cmd->viewangles = vFinalPos;

	if (!gOpts.aimbot_silent)
		I::Engine->SetViewAngles(vFinalPos);

	if(gOpts.aimbot_auto_fire)
		cmd->buttons |= IN_ATTACK;

	Maths::FixSf(cmd, vmov, fmov, smov);

	if (willshoot)
	{
		for (int i = 0; i <= I::Engine->GetMaxClients(); i++)
		{
			if (g_GameEvents.shouldincrementresolvermode[i] == true)
			{
				//Gets called when I hit;
				g_GameEvents.modes[i]++;

				if (g_GameEvents.modes[i] > 2)
					g_GameEvents.modes[i] = 0;

				g_GameEvents.shouldincrementresolvermode[i] = false;
			}
		}

		willshoot = false;
	}
}