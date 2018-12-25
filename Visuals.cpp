#include "Visuals.hpp"
#include "UtlTrace.hpp"
#include "Backtracking.hpp"
#include "Game.hpp"
#include <public/dlight.h>
#include <public/iefx.h>
#include <public/steam/isteamuser.h>
#include <public/steam/isteamfriends.h>
#include <game/shared/igamemovement.h>
#include "Utils.hpp"

CVisuals g_Visuals;

void CreateVisuals(c_baseplayer* entity, bool CSS)
{
	auto local = GetBasePlayer(me);

	if (!local || entity->Health() < 1) return;

	gDrawManager.SetFont(gDrawManager.m_System);

	Color clr = Color(142, 157, 255, 255);

	const matrix3x4 &trans = entity->GetRgflCoordinateFrame();

	Vector min = entity->GetCollMins();
	Vector max = entity->GetCollMaxs();

	Vector pointList[] = {
		Vector(min.x, min.y, min.z), Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z), Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z), Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z), Vector(max.x, min.y, max.z) };

	Vector transformed[8];

	for (int i = 0; i < 8; i++)
		Maths::VectorTransform(pointList[i], trans, transformed[i]);

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if (!gDrawManager.WorldToScreen(transformed[3], flb) || !gDrawManager.WorldToScreen(transformed[0], blb) || !gDrawManager.WorldToScreen(transformed[2], frb) || !gDrawManager.WorldToScreen(transformed[6], blt) || !gDrawManager.WorldToScreen(transformed[5], brt) || !gDrawManager.WorldToScreen(transformed[4], frt) || !gDrawManager.WorldToScreen(transformed[1], brb) || !gDrawManager.WorldToScreen(transformed[7], flt))
		return;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	int m_iHP = entity->Health();

	//Clamp;
	clamp<int>(m_iHP, 0, entity->GetMaxHealth());

	int side_index = 0;

	for (int i = 0; i < 8; i++) {
		if (left > arr[i].x) {
			left = arr[i].x;
		}
		if (top < arr[i].y) {
			top = arr[i].y;
		}
		if (right < arr[i].x) {
			right = arr[i].x;
		}
		if (bottom > arr[i].y) {
			bottom = arr[i].y;
		}
	}

	float x = left;
	float y = bottom;
	float w = right - left;
	float h = top - bottom;
	
	Color GetColor(int HP) {
		
		auto _retn = {0, 0, 0, 255}; //Black;
		
		if(HP > 0 && hp < 25){
			_retn = Color::LightRed();
		} else if(HP > 25 && HP < 50) {
			_retn = Color::LightYellow();
		} else if(HP > 50 && HP < 75) {
			_retn = Color::DefaultOrange();
		} else if(HP > 75){
			_retn = Color::DarkGreen();
		}
		
		return _retn;
	}

	if (gOpts.esp_draw_outline) {
		if (!gOpts.esp_draw_corner_outline) {
			gDrawManager.OutlineRect(x - 1, y - 1, w + 2, h + 1, COLOR_BLACK);
			gDrawManager.OutlineRect(x, y, w, h - 1, clr);
		}
		else {
			gDrawManager.DrawCornerBox(x - 1, y - 1, w + 2, h + 1, 3, 5, COLOR_BLACK);
			gDrawManager.DrawCornerBox(x, y, w, h - 1, 3, 5, clr);
		}
	}

	if (gOpts.esp_draw_names) {
		player_info_t info;

		I::Engine->GetPlayerInfo(entity->GetIndex(), &info);

		gDrawManager.DrawString(x, y - 14 - side_index, clr, info.name);
		side_index += 15;
	}

	if (gOpts.esp_draw_health_bar) {
		int healthBar = (float)h / (float)entity->GetMaxHealth() *(float)m_iHP;
		int healthBarDelta = h - healthBar;

		gDrawManager.OutlineRect(x - 5, y - 1, 5, h + 2, COLOR_BLACK);
		gDrawManager.DrawRect(x - 4, y + healthBarDelta, 3, healthBar, utils::redGreenGradiant(m_iHP, entity->GetMaxHealth()));
	}

	if (gOpts.esp_draw_health_text) {

		char buf[512] = {};

		sprintf(buf, "Health: ");
		sprintf(buf, "%i", m_iHP);
		sprintf(buf, " / ");
		sprintf(buf, "%i", entity->GetMaxHealth());

		gDrawManager.DrawString(x, y - 14 - side_index, utils::redGreenGradiant(m_iHP), buf);
		side_index += 15;

		memset(buf, '\0', sizeof(buf));
	}

	if (gOpts.esp_sight_lines) {
		Vector start, view_angles, w2s0, w2s1;

		start = entity->EyePosition();

		view_angles = (Vector&)entity->GetEyeAngles();

		Vector fwd;

		Maths::AngleVectors(view_angles, &fwd);

		Vector end = (start + fwd);

		if (gDrawManager.WorldToScreen(start, w2s0) && gDrawManager.WorldToScreen(end, w2s1)) {
			gDrawManager.DrawLine(w2s0.x, w2s0.y, w2s1.x, w2s1.y, clr);
		}
	}
	gDrawManager.SetFont(gDrawManager.m_System);
}

void CVisuals::run()
{
#ifdef _HL2DM_MP_v1
#else
	if (I::Engine->GetAppId() == 355180)
	{
		for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++)
		{
			auto entity = GetBasePlayer(i);

			if (!entity || entity->Alive() != true || entity->IsDormant())
				continue;

			if (entity->GetClientClass()->iClassID == 1)
				CreateVisuals(entity, false);
		}

		for (int i = 0; i <= I::Engine->GetMaxClients(); i++)
		{
			auto entity = GetBasePlayer(i);

			if (!entity || entity->Alive() != true || entity->IsDormant())
				continue;

			CreateVisuals(entity, false);
		}
	}

	if (I::Engine->GetAppId() == 240)
	{
		for (int i = 0; i <= I::Engine->GetMaxClients(); i++)
		{
			auto entity = GetBasePlayer(i);

			if (!entity || entity->Alive() != true || entity->IsDormant())
				continue;

			//Postp;
			if (i == me || !gOpts.esp_active)
				continue;

			CreateVisuals(entity, true);
		}
	}

	if (I::Engine->GetAppId() == 440)
	{
		for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++)
		{
			auto entity = GetBasePlayer(i);

			if (!entity || entity->Alive() != true || entity->IsDormant())
				continue;

			//Postp.

			/*			if (gOpts.visuals_world) {
				Vector center, centerws;

				entity->GetWorldSpaceCenter(center);

				std::string mdlName = "";

				mdlName.append(I::MdlInfo->GetModelName((model_t*)entity->GetModel()));

				if (gDrawManager.WorldToScreen(center, centerws)) {

					//Medium Medkit;
					if (mdlName.find("medkit") != string::npos && mdlName.find("medium") != string::npos) {
						gDrawManager.DrawString(centerws.x, centerws.y, Color(227, 252, 37, 255), "Medkit");
					}

					//Medium Medkit;
					if (mdlName.find("ammopack") != string::npos && mdlName.find("medium") != string::npos) {
						gDrawManager.DrawString(centerws.x, centerws.y, Color(227, 252, 37, 255), "Ammopack");
					}
				}
			}*/

			if (i == me || entity->IsPlayer() != true || !gOpts.esp_active)
				continue;

			CreateVisuals(entity, false);
		}
	}

	if (gOpts.esp_local_player)
	{
		auto local = GetBasePlayer(me);

		if (local && local->Alive() && local->IsDormant() != true)
			CreateVisuals(local, true);
	}

#endif
}