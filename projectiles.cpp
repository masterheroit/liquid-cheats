#include "projectiles.hpp"

C_BaseProjectileHelpers* Projectiles::HelpersBase;

float C_BaseProjectileHelpers::acquire_gravity(CBaseWeapon* pWpn)
{
	if (!pWpn) return 0.f;

	int m_pWeaponId = pWpn->GetClientClass()->iClassID;

	auto gravity = static_cast<ConVar*>(I::CVars->FindVar("sv_gravity"));

	if (!gravity) return 0.f;

	switch (m_pWeaponId)
	{
	default:
		return 0.f;
		break;
	}

	//TODO: ADD GRAVITY SUPPORT;
}

void C_BaseProjectileHelpers::predict_vector(c_baseplayer* target, CBaseWeapon* pWpn, Vector in, Vector& out)
{
	auto local = GetBasePlayer(me);

	if (!local || !pWpn) return;

	auto m_flWeaponSpeed = (float)(this->get_projectile_speed(pWpn->GetItemIndex()));
	auto m_vecVelocity = (Vector)(this->acquire_absolute_velocity(target));
	auto m_flProjTimes = (float)((((float)(local->EyePosition().DistTo(in)) / m_flWeaponSpeed)));

	out = in;

	if (target->GetVelType() != FALL_NONE)
		
	{
		//Falling;
		out -= (0.5 * 800.f * powf(m_flProjTimes, 2) * I::Globals->frametime);

		out += (m_vecVelocity * m_flProjTimes);
	}
	else
	{
		out += (m_vecVelocity * m_flProjTimes);
	}
}

float C_BaseProjectileHelpers::get_projectile_speed(int iWpn)
{
	switch (iWpn)
	{
	case WPN_DirectHit:
		return 1980.0f;
		break;

	case WPN_RocketLauncher:
		return 1100.0f;
		break;
	default:
		-1.0f;
		break;
	}
}

Vector C_BaseProjectileHelpers::acquire_absolute_velocity(c_baseplayer* ent)
{
	typedef void(__thiscall * EstimateAbsVelocityFn)(CBaseEntity *, Vector &);

	static DWORD dwFn = sigs::GetClientSignature("E8 ? ? ? ? F3 0F 10 4D ? 8D 85 ? ? ? ? F3 0F 10 45 ? F3 0F 59 C9 56 F3 0F 59 C0 F3 0F 58 C8 0F 2F 0D ? ? ? ? 76 07") + 0x1;

	static DWORD dwEstimate = ((*(PDWORD)(dwFn)) + dwFn + 4);

	EstimateAbsVelocityFn vel = (EstimateAbsVelocityFn)dwEstimate;

	Vector v;

	vel(ent, v);

	return v;
}