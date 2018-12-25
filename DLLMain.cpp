#include "Interfaces.hpp"
#include "SDK.h"
#include "Client.h"
#include "Panels.h"
#include "LAI.hpp"
#include "Glow.hpp"
#include "LCache.hpp"
#include "Backtracking.hpp"
#include "Game.hpp"
#include <interface.h>
#include <public/steam/isteamfriends.h>
#include <public/steam/isteamuser.h>
#include <public/iefx.h>
#include <tier0/mem.h>
#include <d3dx9.h>
#include "LagCompensation.hpp"
#include "DSDK.hpp"
#include "InfoPanel.hpp"
#include "projectiles.hpp"
#include "d3dsprite.h"
#include "Radar.hpp"
#include "texturemgr.h"
#include "KeyMgr.hpp"
#include "AntiAim.hpp"
#include "GameEvents.hpp"
#include "SEGlow.hpp"

IBaseCache* g_Cache;
COffsets gOffsets;
CPlayerVariables gPlayerVars;
CVars gOpts;

CreateInterface_t ValveFactory = NULL;
CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUIFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;
CreateInterface_t VStdFactory = NULL;
CreateInterface_t MatSystemFactory = NULL;

void DrawTexture(IDirect3DDevice9* d3ddev, int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE sprite)
{
	sprite->Draw(dTexture, NULL, NULL, &D3DXVECTOR3(x, y, 0.0f), 0xFFFFFFFF);
	return;
}
void DrawImage(IDirect3DDevice9* d3ddev, int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE sprite, BYTE MyImage)
{
	if (dTexture == NULL)D3DXCreateTextureFromFileInMemory(d3ddev, &MyImage, sizeof(MyImage), &dTexture);
	if (sprite == NULL)D3DXCreateSprite(d3ddev, &sprite); //sprite
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	DrawTexture(d3ddev, x, y, dTexture, sprite);
	sprite->End();
}

#pragma comment(lib, "d3dx9.lib")

#include <Color.h>
void ForceMaterial1(IMaterial* mat, Color color)
{
	if (!mat) return;
	I::MdlRender->ForcedMaterialOverride(mat, OVERRIDE_NORMAL);
}

template < class T >
void add_info(CRender* render, const char* name, T t, int& index)
{
	const char* __charpath = g_info_panel.string_transform<T>(t);

	render->RenderText(g_info_panel.pitch + 1, g_info_panel.yaw + 154 + index, D3DCOLOR_ARGB(255, 255, 255, 255), name);
	render->RenderText(g_info_panel.pitch + 1 + 100, g_info_panel.yaw + 154 + index, D3DCOLOR_ARGB(255, 255, 255, 255), to_string(t).c_str());

	index += 15;
}

void add_info(CRender* render, const char* name, const char* value, int& index)
{
	render->RenderText(g_info_panel.pitch + 1, g_info_panel.yaw + 154 + index, D3DCOLOR_ARGB(255, 255, 255, 255), name);
	render->RenderText(g_info_panel.pitch + 1 + 100, g_info_panel.yaw + 154 + index, D3DCOLOR_ARGB(255, 255, 255, 255), value);

	index += 15;
}

void InitCs()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

void PrintCs(const char* ptr, int mod)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mod > 0 ? FOREGROUND_RED : FOREGROUND_INTENSITY);
	cout << ptr << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
}

template <class T>
void PrintCs(const char* ptr, T t, int mod)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mod > 0 ? FOREGROUND_RED : FOREGROUND_INTENSITY);
	cout << ptr << "" << t << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
}

DWORD WINAPI dwContentThread(LPVOID lpArguments)
{
	gDrawManager.SetFont(gDrawManager.m_System); //Set Font;
	std::string svf = "";

	svf.append(G::server_version);

	if (svf.find("Fastlane") == string::npos || svf.find("1.1") == string::npos) {

		static int recv_counter = 0;

		I::AuthAPI->m_AuthSucceeded = false;

		gOpts.auth_failure = true;

		Sleep(500);

		I::AuthAPI->thread(NULL);

		return;
	}
	else {
		I::AuthAPI->m_AuthSucceeded = true;

		gOpts.auth_failure = false;
	}

	if (gOpts.auth_failure != true) {
	}

	return NULL;
}

DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
#ifdef __FASTDEBUG_LCC
	InitCs();
	std::cout << "FASTDEBUG" << endl;
#else

	if (I::Client == NULL)
	{
		ClientFactory = (CreateInterfaceFn)GetProcAddress(sigs::GetModuleHandleSafe("client.dll"), "CreateInterface");
		EngineFactory = (CreateInterfaceFn)GetProcAddress(sigs::GetModuleHandleSafe("engine.dll"), "CreateInterface");
		VGUIFactory = (CreateInterfaceFn)GetProcAddress(sigs::GetModuleHandleSafe("vguimatsurface.dll"), "CreateInterface");
		MatSystemFactory = (CreateInterfaceFn)GetProcAddress(sigs::GetModuleHandleSafe("MaterialSystem.dll"), "CreateInterface");
		VStdFactory = (CreateInterfaceFn)GetProcAddress(sigs::GetModuleHandleSafe("vstdlib.dll"), "CreateInterface");
		ValveFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA("SteamClient.dll"), "CreateInterface");

		I::Client = (CHLClient*)ClientFactory("VClient017", NULL);
		I::EntList = (CEntList*)ClientFactory("VClientEntityList003", NULL);
		I::Engine = (EngineClient*)EngineFactory("VEngineClient013", NULL);
		I::Surface = (ISurface*)VGUIFactory("VGUI_Surface030", NULL);
		I::MdlInfo = (IVModelInfo*)EngineFactory("VModelInfoClient006", NULL);
		I::Trace = (IEngineTrace*)EngineFactory("EngineTraceClient003", NULL);
		I::Globals = 0x0;0x0;
		I::Prediction = (CPrediction*)ClientFactory("VClientPrediction001", NULL);
		I::CVars = (ICvar*)VStdFactory("VEngineCvar004", NULL);
		I::Input = 0x0;
		I::PlayerInfo = static_cast<CPlayerInformation*>(new CPlayerInformation());
		I::MdlRender = (CModelRender*)EngineFactory("VEngineModel016", NULL);
#ifdef _HL2DM_MP_v1
		I::MdlSys = (CMaterialSystem*)MatSystemFactory("VMaterialSystem080", NULL);
#else
		I::MdlSys = (CMaterialSystem*)MatSystemFactory("VMaterialSystem081", NULL);
#endif
		I::ClientState = 0x0;
		I::EventManager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);
		I::Movement = (CGameMovement*)ClientFactory("GameMovement001", NULL);
		I::EngineServer = (IVEngineServer*)EngineFactory("VEngineServer023", NULL);
		I::SteamClient = (ISteamClient*)ValveFactory("SteamClient017", NULL);
		I::RenderView = (CRenderView*)EngineFactory("VEngineRenderView014", NULL);
		g_Cache = new IBaseCache();
		I::SSEMgr = 0x0;
		g_Cache->cache = new CBaseCache();
		g_Btdata = new CData();
		g_Btdata->data = new CRaw();
		I::Cursors_SizeAll = LoadCursorA(NULL, IDC_SIZEALL);
		InitCs();

		//DumpWeapons();
		//DumpSig2Offs();

		DWORD dwClientModeAddress = sigs::GetClientSignature("8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC");

		I::SSEMgr->InitScreenSpaceEffects();
		I::SSEMgr->EnableScreenSpaceEffect(g_SEGlow.GetGlowMgr());

		I::ClientMode = **(ClientModeShared***)(dwClientModeAddress + 0x2);

#ifndef _HL2DM_MP_v1
		pGlow_t.init();
#endif

		cout << "*\*******************************/*" << endl
			<< "			Interfaces " << endl
			<< "*\*********************************/" << endl
			<< "NOTE: Raw Offsets Only, use for dissasembly in reclass or ida" << endl
			<< endl;

		cout << "Client				:		0x" << I::Client << endl;
		cout << "ClientMode			:		0x" << I::ClientMode << endl;
		cout << "ClientState		:		0x" << I::ClientState << endl;
		cout << "VstdCvars			:		0x" << I::CVars << endl;
		cout << "EngineClient		:		0x" << I::Engine << endl;
		cout << "EngineServer		:		0x" << I::EngineServer << endl;
		cout << "EntityLists		:		0x" << I::EntList << endl;
		cout << "EventManager		:		0x" << I::EventManager << endl;
		cout << "Globals			:		0x" << I::Globals << endl;
		cout << "MoveHelper		:		0x" << I::MoveHelper << endl;

		HSteamPipe hInstPipe = I::SteamClient->CreateSteamPipe();
		HSteamPipe hCurLogon = I::SteamClient->ConnectToGlobalUser(hInstPipe);

		if (I::Engine->GetAppId() == 440) {
			g_LagCompensation.Initialize();

			I::SteamFriends = reinterpret_cast<ISteamFriends*>(I::SteamClient->GetISteamFriends(hCurLogon, hInstPipe, STEAMFRIENDS_INTERFACE_VERSION));
			I::SteamUser = reinterpret_cast<ISteamUser*>(I::SteamClient->GetISteamUser(hCurLogon, hInstPipe, STEAMUSER_INTERFACE_VERSION));

			g_Cache->cache->val.resize(1024);

			if (!I::Panels)
			{
				VGUI2Factory = (CreateInterfaceFn)GetProcAddress(sigs::GetModuleHandleSafe("vgui2.dll"), "CreateInterface");
				I::Panels = (IPanel*)VGUI2Factory("VGUI_Panel009", NULL);
				XASSERT(I::Panels);

				if (I::Panels)
				{
					G::panelHook.setup(I::Panels);
					G::panelHook.hook_index(gOffsets.iPaintTraverseOffset, Hooked_PaintTraverse);
				}
			}

			G::clientModeHook.setup(I::ClientMode);
			G::clientModeHook.hook_index(16, Hooked_OverrideView);

			G::clientHook.setup(I::Client);
			G::clientHook.hook_index(35, Hooked_FSN);
			G::clientHook.hook_index(21, Hooked_CHLCreateMove);

			G::renderHook.setup(I::MdlRender);
			G::renderHook.hook_index(19, Hooked_DrawModelExecute);

			G_Materials* mats = new G_Materials();

			mats->CreateMaterials();
			G::Flat = (CMaterial*)I::MdlSys->FindMaterial("texture-shaded", "Model textures");

			G::clientHook.hook_index(23, &Hooked_WriteUserCmdDeltaToBuffer);

			/*		VMTBaseManager* renderHook = new VMTBaseManager();
			renderHook->Init(I::MdlRender);
			renderHook->HookMethod(&Hooked_DrawModelExecute, 19);
			renderHook->Rehook();

			VMTBaseManager* predHook = new VMTBaseManager();
			predHook->Rehook();*/

			G::predHook.setup(I::Prediction);
			G::predHook.hook_index(17, Hooked_RunCommand);

			g_SEGlow.Init();
		}
		else if (I::Engine->GetAppId() == 320) {
			cout << "HL2DM Initialized!" << endl;

			G::clientModeHook.setup(I::ClientMode);
			G::clientModeHook.hook_index(gOffsets.iCreateMoveOffset, Hooked_CreateMove);
			G::clientModeHook.hook_index(16, Hooked_OverrideView);


			G::predHook.setup(I::Prediction);
			G::predHook.hook_index(17, Hooked_RunCommand);

			G::clientHook.setup(I::Client);
			G::clientHook.hook_index(35, Hooked_FSN);
			G::clientHook.hook_index(21, Hooked_CHLCreateMove);

			G::renderHook.setup(I::MdlRender);
			G::renderHook.hook_index(19, Hooked_DrawModelExecute);

			if (!I::Panels)
			{
				VGUI2Factory = (CreateInterfaceFn)GetProcAddress(sigs::GetModuleHandleSafe("vgui2.dll"), "CreateInterface");
				I::Panels = (IPanel*)VGUI2Factory("VGUI_Panel009", NULL);
				XASSERT(I::Panels);

				if (I::Panels)
				{
					G::panelHook.setup(I::Panels);
					G::panelHook.hook_index(gOffsets.iPaintTraverseOffset, Hooked_PaintTraverse);
				}
			}
		}
		
		while (true) {

			bool bUnload = gOpts.unload;

			if (GetAsyncKeyState(VK_DELETE) & 0x1) bUnload = true;

			Sleep(1000);

			if (bUnload) {
				auto nc = (CNetChan*)I::Engine->GetNetChannelInfo();

				G::clientHook.unhook_all();

				G::clientModeHook.unhook_all();

				G::panelHook.unhook_all();

				G::predHook.unhook_all();

				G::renderHook.unhook_all();

				g_GameEvents_manager.Unload();

				Sleep(1000);

				FreeLibraryAndExitThread(G::hDll, true);

				return FALSE;
			}
		}
	}
#endif
	return 0; //The thread has been completed, and we do not need to call anything once we're done. The call to Hooked_PaintTraverse is now our main thread.
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	//If you manually map, make sure you setup this function properly.
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		Log::Init(hInstance);
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL ); //CreateThread > _BeginThread. (For what we're doing, of course.)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dwContentThread, NULL, 0, NULL); //CreateThread > _BeginThread. (For what we're doing, of course.)

		g_KeyMgr.hInst = hInstance;
		g_KeyMgr.iProcId = NULL;

		G::hDll = (HMODULE)hInstance;
	}
	if (dwReason == DLL_PROCESS_DETACH) {
	}
	return true;
}