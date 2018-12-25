#include "Generals.hpp"

Generals g_Generals;

void Generals::run(bool& bsp, CUserCmd* cmd)
{
	//Forward Declarations For Further Use;
	auto		local		=			(c_baseplayer*)						GetBasePlayer(me);
	auto		netchan		=			(INetChannel*)							I::Engine->GetNetChannelInfo();
	auto		engine		=			/*Default Base*/					I::Engine;
	auto		entlist		=			/*Default Base*/					I::EntList;
	auto&		view		=			/*Default Base*/					cmd->viewangles;
	auto&		buttons		=			/*Default Base*/					cmd->buttons;

	//Initializations Off Required Pointers;
	auto		svcheats	=			/*Default Base*/					static_cast<ConVar*>(I::CVars->FindVar("sv_cheats"));
	auto		camideal	=			/*Default Base*/					static_cast<ConVar*>(I::CVars->FindVar("cam_idealyaw"));

	if (!local || !gOpts.generals_active)
		return;

	if (svcheats->GetInt() < 1)
		svcheats->SetValue(1);

	if (gOpts.thirdperson_menu_active) {
		I::Engine->ClientCmd_Unrestricted("thirdperson");
	}
	else {
		I::Engine->ClientCmd_Unrestricted("firstperson");
	}
}