#include "SDK.h"
#include "Client.h"
#include "CBasePlayer.hpp"
#include "Aim.hpp"
#include "Prediction.hpp"
#include <intrin.h>
#include "AntiAim.hpp"
#include "NetChannel.hpp"
#include "LAI.hpp"
#include "Generals.hpp"
#include "Glow.hpp"
#include "LCache.hpp"
#include "Backtracking.hpp"
#include <inetchannel.h>
#include <inetchannelinfo.h>
#include "GameEvents.hpp"
#include <inetchannel.h>
#include "CNetChan.hpp"
#include "Utils.hpp"
#include "InfoPanel.hpp"
#include <d3dx9.h>
#include "tp_angle.hpp"
#include "UTLDM.hpp"
#include <inetmessage.h>
#include "LagCompensation.hpp"

CreateMoveFn oCreateMove;
FrameStageNotifyFn oFrameStageNotify;
RunCommandFn oRunCommand;
DrawModelExecuteFn oDrawModelExecute;
CreateMoveCmFn oCreateMoveCm;
OverrideViewFn oOverrideView;

#include "KeyMgr.hpp"

QAngle fake_angles;
QAngle real_angles;

vfunc_hook net_chan;

typedef bool(__thiscall* ProcessPacketFn)(CNetChan*, netpacket_s*, bool);
typedef int(__thiscall* SendNetMsgFn)(CNetChan*, INetMessage&, bool, bool);

ProcessPacketFn oProcPacket;
SendNetMsgFn oSendNetMsg;


int __fastcall Hooked_SendNetMsg(CNetChan* netchan, int edx, INetMessage &msg, bool bForceReliable, bool bVoice) {
	oSendNetMsg = net_chan.get_original<SendNetMsgFn>(40);

	if (netchan) {
		const char* name = msg.GetName();

		if (!name) return oSendNetMsg(netchan, msg, bForceReliable, bVoice);
		**reinterpret_cast<CInput ***>((*(DWORD **)I::Client)[15] + 0x2);**reinterpret_cast<CInput ***>((*(DWORD **)I::Client)[15] + 0x2);**reinterpret_cast<CInput ***>((*(DWORD **)I::Client)[15] + 0x2);**reinterpret_cast<CInput ***>((*(DWORD **)I::Client)[15] + 0x2);**reinterpret_cast<CInput ***>((*(DWORD **)I::Client)[15] + 0x2);**reinterpret_cast<CInput ***>((*(DWORD **)I::Client)[15] + 0x2);

		if (gOpts.accuracy_no_angle_fix) {
			if (name[4] == 'F' && name[7] == 'A' && name[10] == 'l') {
				return 1;
			}
		}
	}

	return 	oSendNetMsg(netchan, msg, bForceReliable, bVoice);
}

bool __fastcall Hooked_ProcessPacket(CNetChan* netchan, int edx, netpacket_s* pack, bool header) {
	oProcPacket = net_chan.get_original<ProcessPacketFn>(39);

	oProcPacket(netchan, pack, header);
	
	return true;
}

bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCmd)
{
	if (gOpts.unload) {
		net_chan.unhook_all();
		return false;
	}

	if (gOpts.auth_failure) return false;

	oCreateMoveCm = G::clientModeHook.get_original<CreateMoveCmFn>(gOffsets.iCreateMoveOffset);

	if (!pCmd || pCmd->command_number == NULL || !I::MoveHelper) return false;

	c_baseplayer* local = (c_baseplayer*)GetBasePlayer(me);

	float oc = I::Globals->curtime;
	float of = I::Globals->frametime;

	if (gOpts.generals_active && gOpts.generals_bunnyhop)
		if (!(local->iFlags() & FL_ONGROUND) && pCmd->buttons & IN_JUMP)
			pCmd->buttons &= ~IN_JUMP;

	g_pBasePrediction.start(local, pCmd);

	g_pPredData008.new_eyepos = local->EyePosition();

	g_GameEvents_manager.Init();

	g_Btdata->run(pCmd);

	g_Aim.run(pCmd);
	g_AntiAims.run(G::SendPackets, pCmd);

	utldm::predict_barrel(pCmd);

	g_Generals.run(G::SendPackets, pCmd);

	g_pBasePrediction.enddh(local, pCmd,oc, of);

	oCreateMoveCm(ClientMode, input_sample_frametime, pCmd);

	if (gOpts.thirdperson_menu_active) {
		real_angles = (QAngle&)pCmd->viewangles;
	}

	return false;
}

void __fastcall Hooked_CHLCreateMove(PVOID CHLClient, int edx, int sequence_number, float input_sample_time, bool active)
{
	g_LagCompensation.run();

	if (gOpts.unload) {
		net_chan.unhook_all();
		return;
	}

	static bool didHook = false;

	static auto __netchan = (CNetChan*)nullptr;

	auto netchan = (CNetChan*)I::Engine->GetNetChannelInfo();

	if (netchan && __netchan != netchan) {

		__netchan = netchan;

		net_chan.setup(netchan);
		net_chan.hook_index(39, Hooked_ProcessPacket);
		net_chan.hook_index(40, Hooked_SendNetMsg);
	}

	if (gOpts.auth_failure) return;

	oCreateMove = G::clientHook.get_original<CreateMoveFn>(gOffsets.iCreateMoveOffset);

	int _ebp = 0;

	__asm {
		mov _ebp, ebp;
	}

	auto *bSendPacket = (bool *)(*(char **)_ebp - 1);

	oCreateMove(CHLClient, sequence_number, input_sample_time, active);

	G::SendPackets = true;

#ifndef _HL2DM_MP_v1

	g_KeyMgr.Init();

	CUserCmd *cmd = &I::Input->m_pCommands[sequence_number % 90];
	CVerifiedUserCmd *m_pVerifiedCommands = I::Input->m_pVerifiedCommands;
	CVerifiedUserCmd *verified_cmd = &m_pVerifiedCommands[sequence_number % 90];

	c_baseplayer* local = (c_baseplayer*)GetBasePlayer(me);

	if (!cmd || !cmd->command_number || !local || !I::MoveHelper) return;

	float oc = I::Globals->curtime;
	float of = I::Globals->frametime;

	auto g_pPrediction = static_cast<G_Prediction*>(new G_Prediction());

	if (gOpts.generals_active && gOpts.generals_bunnyhop) {
		static bool firstjump = 0, fakejmp;

		if (cmd->buttons & IN_JUMP)
			if (!firstjump)
				firstjump = fakejmp = 1;
			else if (!(local->iFlags() & FL_ONGROUND))
				if (fakejmp)
					fakejmp = 0;
				else
					cmd->buttons &= ~IN_JUMP;
			else
				fakejmp = 1;
		else
			firstjump = 0;
	}

	g_pPrediction->start_prediction(local, cmd);

	g_pPredData008.new_eyepos = local->EyePosition();

	g_GameEvents_manager.Init();

	g_Btdata->run(cmd);

	g_Cache->cache->local = local;
	g_Cache->cache->cmd = cmd;

	g_Aim.run(cmd);
	g_AntiAims.run(G::SendPackets, cmd);

	g_Generals.run(G::SendPackets, cmd);

	if (verified_cmd)
	{
		verified_cmd->m_cmd = *cmd;
		verified_cmd->m_crc = cmd->GetChecksum();
	}

	g_pPrediction->end_prediction(local, oc, of);

	*bSendPacket = G::SendPackets;

	if (gOpts.thirdperson_menu_show_fake_angles)
		if (*bSendPacket)
			fake_angles = (QAngle&)cmd->viewangles;

	if (gOpts.thirdperson_menu_show_real_angles)
		if (*bSendPacket != true)
			real_angles = (QAngle&)cmd->viewangles;
#endif
}

void __fastcall Hooked_FSN(PVOID CHLClient, void *_this, ClientFrameStage_t _clientframestage)
{
	oFrameStageNotify = G::clientHook.get_original<FrameStageNotifyFn>(35);

	if (gOpts.unload) return oFrameStageNotify(CHLClient, _this, _clientframestage);

	//Stuff Before Validation;
	auto local = (c_baseplayer*)GetBaseEntity(me);

	if (!local || local->Alive() != true) {
		return oFrameStageNotify(CHLClient, _this, _clientframestage);
	}

	//Stuff After Validation;
	int curCond = local->GetConds();

	switch (_clientframestage) {
	case FRAME_RENDER_START: //Start the frame;
	{
		///Scope Playermodel;
		g_Cache->cache->scoped = (local->GetSequenceNr() == 212 || local->GetSequenceNr() == 209 || local->GetSequenceNr() == 210 || local->GetSequenceNr() == 211 && utils::check_scoped(local));

		if (g_Cache->cache->scoped) {
			local->set_cond(curCond & ~tf_cond::TFCond_Zoomed);
		}
		///....;

		///Thirdperson Viewangles;
		if (I::Input->CAM_IsThirdPerson())
		{
			DWORD dwDeadFlagLocation = g_Netvars.m_dwDeadFlag;

			float* lastpitch;
			float* lastyaw;

			lastpitch = local->CreatePtr_t<float>(dwDeadFlagLocation + 4);
			lastyaw = local->CreatePtr_t<float>(dwDeadFlagLocation + 8);

			*lastpitch = fake_angles.x;
			*lastyaw = fake_angles.y;
		}
		///....;
	}
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START: //Postprocess x1;
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END: //Postprocess x2;
		break;
	case FRAME_NET_UPDATE_START: //Update the frame;
		break;
	case FRAME_NET_UPDATE_END: //Finish the update;
		break;
	case FRAME_RENDER_END: //End the frame;
		break;
	case FRAME_UNDEFINED: //Undefined frame;
		break;
	default:
		break; //Unknown frame;
	}

	if (!oFrameStageNotify) return;

	return oFrameStageNotify(CHLClient, _this, _clientframestage);
}

void __stdcall Hooked_RunCommand(CBaseEntity *pBaseEntity, CUserCmd *pCommand, IMoveHelper *moveHelper)
{
	oRunCommand = G::predHook.get_original<RunCommandFn>(17);

	if (!I::MoveHelper)
		I::MoveHelper = moveHelper;

	oRunCommand(I::Prediction, pBaseEntity, pCommand, moveHelper);
}

float_t DetermineLocalFOV(float cur_fov, c_baseplayer* ent) {
	if (ent->GetConds() & TFCond_Zoomed) return 0.00f;

	return cur_fov;
}

void __stdcall Hooked_OverrideView(CViewSetup* setup)
{
	oOverrideView = G::clientModeHook.get_original< OverrideViewFn>(16);

	if (!I::Engine->IsInGame() && !I::Engine->IsConnected())
		return;

	auto local = GetBasePlayer(me);

	if (!local) return;

	if (gOpts.accuracy_no_zoom) {
		setup->m_flFov = 89.0f;
	}

	oOverrideView(setup);
}

void __stdcall Hooked_DrawModelExecute(void *state, ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	oDrawModelExecute = G::renderHook.get_original<DrawModelExecuteFn>(19);

	if (gOpts.render_active) {
		std::string pMdlName = "";

		pMdlName.append(I::MdlInfo->GetModelName((model_t*)pInfo.pModel));

		if (gOpts.render_wireframe_explosive_barrels) {
			if (pMdlName.find("oildrum001_explosive") != string::npos) {
				IMaterial* pOildrumMaterial = I::MdlSys->FindMaterial(pMdlName.c_str(), TEXTURE_GROUP_MODEL);

				float pHandsColor[10];

				pHandsColor[0] = 255;
				pHandsColor[1] = 25;
				pHandsColor[2] = 94;

				pOildrumMaterial->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_FLAT, true);
				pOildrumMaterial->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_IGNOREZ, true);

				I::MdlRender->ForcedMaterialOverride(pOildrumMaterial, OVERRIDE_NORMAL);

				I::RenderView->SetColorModulation(pHandsColor);

				oDrawModelExecute(I::MdlRender, state, pInfo, pCustomBoneToWorld);
			}
		}

		if (pMdlName.find("arms") != std::string::npos) {
			IMaterial* pHandMaterial = I::MdlSys->FindMaterial(pMdlName.c_str(), TEXTURE_GROUP_MODEL);

			pHandMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, gOpts.render_wireframe_hands);
			
			float pHandsColor[10];

			if (gOpts.render_colored_hands) {
				pHandsColor[0] = gOpts.render_colored_hands_r;
				pHandsColor[1] = gOpts.render_colored_hands_g;
				pHandsColor[2] = gOpts.render_colored_hands_b;
			}

			if (gOpts.render_no_hands) {
				return;
			}

			I::MdlRender->ForcedMaterialOverride(pHandMaterial, OVERRIDE_NORMAL);

			I::RenderView->SetColorModulation(pHandsColor);

			oDrawModelExecute(I::MdlRender, state, pInfo, pCustomBoneToWorld);
		}
	}

	I::RenderView->SetBlend(1.0f);
	I::MdlRender->ForcedMaterialOverride(nullptr, OVERRIDE_NORMAL);

	oDrawModelExecute(I::MdlRender, state, pInfo, pCustomBoneToWorld);
}

bool __fastcall Hooked_WriteUserCmdDeltaToBuffer(PVOID CHLClient, int edx, bf_write *buf, int from, int to, bool isNewCommand)
{
	using WriteUserCmdFn = void(__cdecl *)(bf_write *, CUserCmd *, CUserCmd *);

	static WriteUserCmdFn writeUserCmd = (WriteUserCmdFn)sigs::GetClientSignature(
		__WRITEUSERCMD_SIG
	);

	CUserCmd nullcmd, *pFrom, *pTo;

	if (from == -1)
	{
		pFrom = &nullcmd;
	}
	else {
		pFrom = I::Input->GetUserCmd(from);

		if (!pFrom)
			pFrom = &nullcmd;
	}

	pTo = I::Input->GetUserCmd(to);

	if (!pTo)
		pTo = &nullcmd;

	writeUserCmd(buf, pTo, pFrom);

	if (*(bool *)((DWORD)buf + 0x10))
		return false;

	return true;
}