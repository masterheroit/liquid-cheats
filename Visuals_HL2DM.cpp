#include "Visuals.hpp"
#include "SEGlow.hpp"

C_HL2DMVisuals HL2DM::g_Visuals;

void C_HL2DMVisuals::barrel_visuals(c_baseplayer* entity) {
	gDrawManager.SetFont(gDrawManager.m_System);

	Color clr = Color(255, 71, 199, 255);

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

	gDrawManager.DrawString(x, y - 14 - side_index, clr, "EXPLOSIVE BARREL");
	side_index -= 10;

	gDrawManager.SetFont(gDrawManager.m_System);
}

void C_HL2DMVisuals::local_visuals(c_baseplayer* entity) {
	gDrawManager.SetFont(gDrawManager.m_System);

	Color clr = Color(255, 71, 199, 255);

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
		side_index -= 10;
	}

	gDrawManager.SetFont(gDrawManager.m_System);
}

void C_HL2DMVisuals::visuals(c_baseplayer* entity) {
	gDrawManager.SetFont(gDrawManager.m_System);

	g_SEGlow.GetGlowMgr()->RegisterEnt(entity->GetIndex(), Color(255, 255, 0, 255), 4.0f);

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
		side_index -= 10;
	}

	gDrawManager.SetFont(gDrawManager.m_System);
}

void C_HL2DMVisuals::run() {
#ifdef _HL2DM_MP_v1
	auto local = GetBasePlayer(me);

	if (!local) return;

	for (int i = 0; i <= I::Engine->GetMaxClients(); i++) {
		auto entity = GetBasePlayer(i);

		if (!entity || !gOpts.esp_active || !gOpts.esp_players || entity->IsDormant() || !(entity->Alive())) continue;

		if (!gOpts.esp_draw_teammates && entity->Team() == local->Team()) continue;
		if (!gOpts.esp_local_player && !gOpts.thirdperson_menu_active && i == me) continue;

		const char* mdlName = I::MdlInfo->GetModelName((model_t*)entity->GetModel());

		//
		// PLAYER ESP (CLIENTS) V1
		//

		this->visuals(entity);

		//
		//
		//
	}

	CBaseWeapon* wpn = local->get_weapon();

	if (!wpn) return;

	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++) {
		c_baseplayer* entity = GetBasePlayer(i);

		if (!entity || entity->IsDormant() || !(entity->Alive())) continue;

		auto physgun = (IBasePhysCannon*)wpn;

		auto mdl = entity->GetModel();

		auto mdl_name = I::MdlInfo->GetModelName((model_t*)mdl);

		Vector center, centerws;

		entity->GetWorldSpaceCenter(center);

		std::string name_t = mdl_name;

		if (name_t.find("oildrum001_explosive") != string::npos) {
			//Reflector;

			this->visuals(entity);

			if (gOpts.trigger_auto_barrel_reflection) {
				auto flDistance = local->EyePosition().DistTo(center);

				int i = 0;

				if (gDrawManager.WorldToScreen(center, centerws)) {

					if (physgun->GetHeldEntity() && physgun->GetHeldEntity() == entity) {
						gDrawManager.DrawString(centerws.x, centerws.y + i, Color(RandomFloatExp(0, 255) * 255, RandomFloatExp(0, 255) * 255, RandomFloatExp(0, 255) * 255, 255), "*BEING HELD*");
						i += 10;
					}

					gDrawManager.DrawString(centerws.x, centerws.y + i, Color(255, 255, 255, 255), mdl_name);
					i += 10;

					gDrawManager.DrawString(centerws.x, centerws.y + i, utldm::barrel_in_range((QAngle&)local->GetEyeAngles(), flDistance) ? Color(55, 255, 55, 255) : Color(255, 255, 255, 255), std::to_string(flDistance).c_str());
					i += 10;
				}
			}
		}
	}

#endif
}