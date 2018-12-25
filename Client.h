#pragma once

#include "SDK.h"
typedef void(__thiscall* CreateMoveFn)(PVOID, int, float, bool);
typedef void(__thiscall* PaintTraverseFn)(PVOID, unsigned int, bool, bool);
typedef void(__fastcall* FrameStageNotifyFn)(PVOID, void*, ClientFrameStage_t);
typedef void(__thiscall* RunCommandFn)(CPrediction*, CBaseEntity*, CUserCmd*, IMoveHelper*);
typedef void(__thiscall* DrawModelExecuteFn)(PVOID, void *, ModelRenderInfo_t &, matrix3x4 *);
typedef int(__thiscall* SendDatagramFn)(INetChannelInfo* netchan, bf_write* datagram);
typedef bool(__thiscall* CreateMoveCmFn)(PVOID, float, CUserCmd*);
typedef void(__stdcall* OverrideViewFn)(CViewSetup*);

extern CreateMoveFn oCreateMove;
extern PaintTraverseFn oPaintTraverse;
extern FrameStageNotifyFn oFrameStageNotify;
extern RunCommandFn oRunCommand;
extern DrawModelExecuteFn oDrawModelExecute;
extern SendDatagramFn oSendDatagram;

extern CreateMoveCmFn oCreateMoveCm;
extern OverrideViewFn oOverrideView;

bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand);
void __fastcall Hooked_FSN(PVOID CHLClient, void *_this, ClientFrameStage_t Stage);
void __stdcall Hooked_DrawModelExecute(void *state, ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
int __stdcall Hooked_InPrediction();

void __stdcall Hooked_RunCommand(CBaseEntity *pBaseEntity, CUserCmd *pCommand, IMoveHelper *moveHelper);
bool __fastcall Hooked_WriteUserCmdDeltaToBuffer(PVOID CHLClient, int edx, bf_write *buf, int from, int to, bool isNewCommand);
void __fastcall Hooked_CHLCreateMove(PVOID CHLClient, int edx, int sequence_number, float input_sample_time, bool active);
void __stdcall Hooked_OverrideView(CViewSetup* setup);