#include "Radar.hpp"
#include "d3dsprite.h"

c_radar g_radar;

int c_radar::pitch = 250;
int c_radar::yaw = 250;
int c_radar::s = 150;

void c_radar::DrawRadarIcon(IDirect3DDevice9* pDevice, Vector origx, Vector origy, QAngle angle, c_baseplayer *ent)
{
	CBaseEntity* local = GetBaseEntity(me);

	if (!local) return;

	float flDeltaX = origx.x - origy.x;
	float flDeltaY = origx.y - origy.y;

	float flAngle = angle.y;

	float flYaw = (flAngle)* (3.14159265358979323846 / 180.0);
	float flMainViewAngles_CosYaw = cos(flYaw);
	float flMainViewAngles_SinYaw = sin(flYaw);

	// rotate
	float x = flDeltaY * (-flMainViewAngles_CosYaw) + flDeltaX * flMainViewAngles_SinYaw;
	float y = flDeltaX * (-flMainViewAngles_CosYaw) - flDeltaY * flMainViewAngles_SinYaw;

	float flRange = 1940;

	if (fabs(x) > flRange || fabs(y) > flRange)
	{
		// clipping
		if (y > x)
		{
			if (y > -x)
			{
				x = flRange * x / y;
				y = flRange;
			}
			else
			{
				y = -flRange * y / x;
				x = -flRange;
			}
		}
		else
		{
			if (y > -x)
			{
				y = flRange * y / x;
				x = flRange;
			}
			else
			{
				x = -flRange * x / y;
				y = -flRange;
			}
		}
	}

	int	iScreenX = this->pitch + int(x / flRange * s);
	int iScreenY = this->yaw + int(y / flRange * s);

	static LPDIRECT3DTEXTURE9 tx = LPDIRECT3DTEXTURE9();

	if (ent->TranslateClassToFile() == "C:\\icons\\radar\\none.png") return;

	Texture::draw(pDevice, ent->GetClassById(), iScreenX - 2, iScreenY - 2, 35, 35);
	Texture::draw(pDevice, ent->GetClassById(), iScreenX - 2, iScreenY - 2, 35, 35);
}

void c_radar::DrawRadarPoint(Vector origx, Vector origy, QAngle angle, c_baseplayer *ent)
{
	c_baseplayer* local = (c_baseplayer*)GetBaseEntity(me);

	if (!local) return;

	float flDeltaX = origx.x - origy.x;
	float flDeltaY = origx.y - origy.y;

	float flAngle = angle.y;

	float flYaw = (flAngle)* (3.14159265358979323846 / 180.0);
	float flMainViewAngles_CosYaw = cos(flYaw);
	float flMainViewAngles_SinYaw = sin(flYaw);

	// rotate
	float x = flDeltaY * (-flMainViewAngles_CosYaw) + flDeltaX * flMainViewAngles_SinYaw;
	float y = flDeltaX * (-flMainViewAngles_CosYaw) - flDeltaY * flMainViewAngles_SinYaw;

	float flRange = gOpts.radar_range * 10;

	if (fabs(x) > flRange || fabs(y) > flRange)
	{
		// clipping
		if (y > x)
		{
			if (y > -x)
			{
				x = flRange * x / y;
				y = flRange;
			}
			else
			{
				y = -flRange * y / x;
				x = -flRange;
			}
		}
		else
		{
			if (y > -x)
			{
				y = flRange * y / x;
				x = flRange;
			}
			else
			{
				x = -flRange * x / y;
				y = -flRange;
			}
		}
	}

	int	iScreenX = this->pitch + int(x / flRange * s);
	int iScreenY = this->yaw + int(y / flRange * s);

	gDrawManager.SetFont(gDrawManager.m_Hl2Mp);

	if (!local->get_weapon() || ent == (CBaseEntity*)local->get_weapon() || ent == local || !ent->Alive()) return;

	auto chID = I::MdlInfo->GetModelName((model_t*)ent->GetModel());

	string str = "";

	str.append(chID);

	Color color = Color(255, 255, 255, 255);

	if (local->get_weapon()->GetClientClass()->iClassID == ent->GetClientClass()->iClassID)
		color = Color(0, 255, 199, 255);

#ifdef _HL2DM_MP_v1
	if (str.find("healthkit") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "5");
	}

	if (str.find("grenade") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "4");
	}

	if (str.find("missile_closed") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "4");
	}

	if (str.find("rocket_launcher") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "3");
	}

	if (str.find("explosive") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "8");
	}

	if (str.find("w_missile") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "7");
	}

	if (str.find("missile_closed") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "4");
	}

	if (str.find("w_slam") != string::npos) {
		gDrawManager.DrawString(iScreenX - 2, iScreenY - 2, color, "*");
	}
#endif

	gDrawManager.SetFont(gDrawManager.m_System);
}

void c_radar::paint_traverse()
{
	this->s = gOpts.radar_size * 10;

	if (!gOpts.radar_active) return;
	/*	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++) {
		gDrawManager.SetFont(gDrawManager.m_Tahoma);

		Vector vec, ang;

		auto entity = GetBaseEntity(i);

		if (!entity) continue;

		entity->GetWorldSpaceCenter(vec);

		auto id = entity->GetClientClass()->iClassID;

		if (id == 123) {

		}

		if (gDrawManager.WorldToScreen(vec, ang)) {
			gDrawManager.DrawString(ang.x, ang.y, Color(255, 255, 255, 255), I::MdlInfo->GetModelName(entity->GetModel()));
		}
	}
	*/

	static bool ok = false;

	if (ok != true) {
		I::Engine->ClientCmdCurrentTick("net_graphheight 100");
		I::Engine->ClientCmdCurrentTick("net_graphpos 2");
		I::Engine->ClientCmdCurrentTick("fps_max 200");
		I::Engine->ClientCmdCurrentTick("net_graph 1");
		ok = true;
	}

	int x, y;

	I::Surface->SurfaceGetCursorPos(x, y);

	bool pitch_pass = false, yaw_pass = false;

	static bool is_moving = false;

	if (x > this->pitch - 150 && x < this->pitch + 150) pitch_pass = true;
	if (y > this->yaw - 150 && y < this->yaw + 150) yaw_pass = true;

	if (pitch_pass && yaw_pass && gOpts.menu_active) //hovering;
	{
		///Handling;
		is_moving = true;
	}

	if (is_moving)
	{
		//Handles;
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			//Moving;
			SetCursor(I::Cursors_SizeAll);

			this->pitch = x;
			this->yaw = y;
		}
		else
		{
			is_moving = false;
		}
	}

	Color background = Color(45, 45, 45, gOpts.radar_background_opacity);
	Color foreground = Color(54, 126, 115, 255);

	gDrawManager.DrawRect(pitch - s, yaw - s, 2 * s + 2, 2 * s + 2, background);
	gDrawManager.OutlineRect(pitch - s, yaw - s, 2 * s + 2, 2 * s + 2, foreground);

	int tmp_s = this->s;

	tmp_s -= 10.02;

	gDrawManager.DrawRect(pitch, yaw - tmp_s - 11, 1, 2 * tmp_s + 22, foreground);
	gDrawManager.DrawRect(pitch - tmp_s - 11, yaw, 2 * tmp_s + 22, 1, foreground);

	auto local = GetBasePlayer(me);

	if (!local) return;

	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++) {
		auto entity = GetBasePlayer(i);

		if (!entity) continue;

		this->DrawRadarPoint(entity->GetAbsOrigin(), local->GetAbsOrigin(), QAngle{ local->GetAbsAngles().x, local->GetAbsAngles().y, local->GetAbsAngles().z }, entity);
	}
}