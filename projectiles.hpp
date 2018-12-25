#include "SDK.h"
#include "CBasePlayer.hpp"

class C_BaseProjectileHelpers //Set Of Basic Standard Projectile Predicition Utilities;
{
public:
	float acquire_gravity(CBaseWeapon* pWpn);
	void predict_vector(c_baseplayer* target, CBaseWeapon* pWpn, Vector in, Vector& out);
	float get_projectile_speed(int iWpn);
	Vector acquire_absolute_velocity(c_baseplayer* entity);
};

namespace Projectiles
{
	extern C_BaseProjectileHelpers* HelpersBase;
}