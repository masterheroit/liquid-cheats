#include "UTLDM.hpp"

bool utldm::barrel_in_range(QAngle ang_local, float barrel_dst) {
	
	bool upped = false;
	bool centered = false;
	bool downed = false;
	bool unknown = false;

	if (ang_local.x >= -50 && ang_local.x <= 50) { //Center;
		centered = true;
	}
	else if (ang_local.x >= -89 && ang_local.x <= +89) { //Down;
		downed = true;
	}
	else if (ang_local.x <= 89 && ang_local.x >= -89.0f) { //Up;
		upped = true;
	}
	else { //Unknown
		unknown = true;
	}

	if (centered && barrel_dst >= 10.5f && barrel_dst <= 101.5f) {
		return true;
	}
	else if (upped && barrel_dst >= 10.0f && barrel_dst <= 150.0f) {
		return true;
	}
	else if (downed && barrel_dst >= 10.0f && barrel_dst <= 100.0f) {
		return true;
	}

	if (unknown && barrel_dst >= 40.5f && barrel_dst <= 101.5f) {
		return true;
	}

	return false;
}

void utldm::predict_barrel(CUserCmd* cmd) {
#ifdef _HL2DM_MP_v1
	if (!gOpts.trigger_auto_barrel_reflection) return;

	auto local = GetBasePlayer(me);

	if (!local) return;

	CBaseWeapon* wpn = local->get_weapon();

	if (!wpn) return;

	auto item_mdl = wpn->GetModel();

	std::string wpn_name = I::MdlInfo->GetModelName((model_t*)item_mdl);

	int cEBI = -1;

	if (wpn_name.find("models/weapons/w_Physics") != string::npos) cEBI = 1;
	if (wpn_name.find("phy") != string::npos) cEBI = 1;

	if (cEBI <= -1) return;

	cout << wpn->GetItemIndex() << endl;

	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++) {
		c_baseplayer* entity = GetBasePlayer(i);

		if (!entity || entity->IsDormant() || !(entity->Alive())) continue;

		auto physgun = (IBasePhysCannon*)wpn;

		auto mdl = entity->GetModel();

		auto mdl_name = I::MdlInfo->GetModelName((model_t*)mdl);

		Vector center, centerws;

		entity->GetWorldSpaceCenter(center);

		std::string name_t = mdl_name;

		static bool process_t = false;

		if (name_t.find("oildrum001_explosive") != string::npos) {
			//Reflector;

			auto flDistance = local->EyePosition().DistTo(center);

			int i = 0;

			Vector oa = cmd->viewangles;

			if (barrel_in_range(local->GetEyeAngles(), flDistance)) {
				Vector va;

				Maths::VectorAngles((center - local->EyePosition()), va);
				cmd->viewangles = va;
				cmd->buttons |= IN_ATTACK;
				cmd->viewangles = oa;
			}
			else {
				cmd->viewangles = oa;
			}
		}
	}

#endif
}