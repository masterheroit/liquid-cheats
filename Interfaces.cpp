#include "Interfaces.hpp"
#include <fstream>

CRenderView* I::RenderView;
IVEngineServer* I::EngineServer;
CEntList* I::EntList;
EngineClient* I::Engine;
IPanel* I::Panels;
ISurface* I::Surface;
ClientModeShared* I::ClientMode;
CHLClient* I::Client;
IVModelInfo* I::MdlInfo;
IEngineTrace* I::Trace;
CPrediction* I::Prediction;
CGlobals* I::Globals;
CClientState* I::CState;
ICvar* I::CVars;
CInput* I::Input;
CPlayerInformation* I::PlayerInfo;
CModelRender* I::MdlRender;
CMaterialSystem* I::MdlSys;
CClientState* I::ClientState;
IGameEventManager2* I::EventManager;
ISteamClient* I::SteamClient;
ISteamFriends* I::SteamFriends;
ISteamUser* I::SteamUser;
CGameMovement* I::Movement;
IMoveHelper* I::MoveHelper;
bool G::SendPackets;
IMaterial* G::Flat;
HCURSOR I::Cursors_SizeAll;
IScreenSpaceEffectManager* I::SSEMgr;
C_AuthAPI* I::AuthAPI

VMTBaseManager G::clientModeHook = new VMTBaseManager(); //Setup our VMTBaseManager for ClientMode.

VMTBaseManager G::clientHook = new VMTBaseManager();

VMTBaseManager G::panelHook = new VMTBaseManager();

VMTBaseManager G::predHook = new VMTBaseManager();

VMTBaseManager G::renderHook = new VMTBaseManager();

VMTBaseManager G::netHook = new VMTBaseManager();

HMODULE G::hDll = NULL;

void G_Materials::CreateMaterials()
{
	std::ofstream("tf\\materials\\texture-shaded.vmt") << R"#("UnlitGeneric" {

"$basetexture" "vgui/white_additive"

"$ignorez" "1"

"$envmap" "0"

"$normalmapalphaenvmapmask" "0"

"$envmapcontrast" "1"

"$nofog" "1"

"$model" "1"

"$nocull" "1"

"$selfillum" "1"

"$halflambert" "1"

"$znearer" "0"

"$flat" "1"

}

)#";
}