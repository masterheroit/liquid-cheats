#pragma once

#ifndef __INTERFACES_HPP
#define __INTERFACES_HPP
#include "AUT.hpp"
#include "SDK.h"

class G_Materials
{
public:
	CMaterial* dflat;
	CMaterial* dflat_v;

	void CreateMaterials();
	void CreateMaterial(bool flat, bool ignorez, bool shadows);
};

class G_Interfaces
{
public:
	CRenderView* RenderView;
	IVEngineServer* EngineServer;
	CEntList* EntList;
	EngineClient* Engine;
	IPanel* Panels;
	ISurface* Surface;
	ClientModeShared* ClientMode;
	CHLClient* Client;
	IVModelInfo* MdlInfo;
	IEngineTrace* Trace;
	CPrediction* Prediction;
	CGlobals* Globals;
	CClientState* CState;
	ICvar* CVars;
	CInput* Input;
	CPlayerInformation* PlayerInfo;
	CModelRender* MdlRender;
	CMaterialSystem* MdlSys;
	CClientState* ClientState;
	IGameEventManager2* EventManager;
	ISteamClient* SteamClient;
	ISteamFriends* SteamFriends;
	ISteamUser* SteamUser;
	CGameMovement* Movement;
	IMoveHelper* MoveHelper;
	IScreenSpaceEffectManager* ScreenSpaceEffectManager;
};

class Globals
{
public:
	bool* bSendPacket;
};

struct prediction_info_t
{
	Vector start_line, end_line;
	float cur_time;
	bool predicted;
	
	Vector legacy_hitbox;
	Vector predicted_hitbox;
};
class PredictionCache
{
public:
	prediction_info_t info[34];
};

namespace I
{
	extern CRenderView* RenderView;
	extern IVEngineServer* EngineServer;
	extern CEntList* EntList;
	extern EngineClient* Engine;
	extern IPanel* Panels;
	extern ISurface* Surface;
	extern ClientModeShared* ClientMode;
	extern CHLClient* Client;
	extern IVModelInfo* MdlInfo;
	extern IEngineTrace* Trace;
	extern CPrediction* Prediction;
	extern CGlobals* Globals;
	extern CClientState* CState;
	extern ICvar* CVars;
	extern CInput* Input;
	extern CPlayerInformation* PlayerInfo;
	extern CModelRender* MdlRender;
	extern CMaterialSystem* MdlSys;
	extern CClientState* ClientState;
	extern IGameEventManager2* EventManager;
	extern ISteamClient* SteamClient;
	extern ISteamFriends* SteamFriends;
	extern ISteamUser* SteamUser;
	extern CGameMovement* Movement;
	extern IMoveHelper* MoveHelper;
	extern HCURSOR Cursors_SizeAll;
	extern IScreenSpaceEffectManager* SSEMgr;
	extern C_AuthAPI* AuthAPI;
}

namespace G
{
	extern bool SendPackets;
	extern IMaterial* Flat;
	extern VMTBaseManager clientModeHook;
	extern VMTBaseManager clientHook;
	extern VMTBaseManager panelHook;
	extern VMTBaseManager predHook;
	extern VMTBaseManager renderHook;
	extern VMTBaseManager netHook;
	extern HMODULE hDll;
	extern string server_version;
}

#endif