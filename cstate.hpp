#include "SDK.h"
#include "cframe.hpp"
#include <basehandle.h>
#include <netadr.h>
#include <utllinkedlist.h>
#include <utlfixedmemory.h>
#include "CNetChan.hpp"

class UpdateType {};
class DataUpdateType_t {};

#define STEAM_KEYSIZE 2048

#define MAX_AREA_STATE_BYTES 32
#define MAX_AREA_PORTAL_STATE_BYTES 24
#define MAX_PLAYER_NAME_LENGTH 32
#define MAX_CUSTOM_FILES 4 // max 4 files
#define MAX_DEMOS 32
#define MAX_DEMONAME 32

// Resource counts;
#define MAX_MODEL_INDEX_BITS 10 // sent as a short
#define MAX_MODELS (1 << MAX_MODEL_INDEX_BITS)

#define MAX_GENERIC_INDEX_BITS 9
#define MAX_GENERIC (1 << MAX_GENERIC_INDEX_BITS)
#define MAX_DECAL_INDEX_BITS 9
#define MAX_BASE_DECALS (1 << MAX_DECAL_INDEX_BITS)

#define MAX_SOUND_INDEX_BITS 14

struct model_t;
class ClientClass;

// dummy pointers for classes we dont want or need
// TODO impl if nesercary
typedef int INetworkStringTable, PackedEntity, C_ServerClassInfo, CNetworkStringTableContainer, CSfxTable, WaitForResourcesHandle_t;

class CEntityInfo
{
public:
	CEntityInfo()
	{
		m_nOldEntity = -1;
		m_nNewEntity = -1;
		m_nHeaderBase = -1;
	}
	virtual ~CEntityInfo() {};

	bool          m_bAsDelta;
	CClientFrame *m_pFrom;
	CClientFrame *m_pTo;

	UpdateType m_UpdateType;

	int m_nOldEntity; // current entity index in m_pFrom
	int m_nNewEntity; // current entity index in m_pTo

	int m_nHeaderBase;
	int m_nHeaderCount;
};

// PostDataUpdate calls are stored in a list until all ents have been updated.
class CPostDataUpdateCall
{
public:
	int              m_iEnt;
	DataUpdateType_t m_UpdateType;
};

class CEntityReadInfo : public CEntityInfo
{

public:
	CEntityReadInfo()
	{
		m_nPostDataUpdateCalls = 0;
		m_nLocalPlayerBits = 0;
		m_nOtherPlayerBits = 0;
		m_UpdateType = UpdateType();
	}

	bf_read *m_pBuf;
	int      m_UpdateFlags; // from the subheader
	bool     m_bIsEntity;

	int  m_nBaseline; // what baseline index do we use (0/1)
	bool m_bUpdateBaselines; // update baseline while parsing snaphsot

	int m_nLocalPlayerBits; // profiling data
	int m_nOtherPlayerBits; // profiling data

	CPostDataUpdateCall m_PostDataUpdateCalls[MAX_EDICTS];
	int                 m_nPostDataUpdateCalls;
};

class CClockDriftMgr
{
	friend class CBaseClientState;

public:
	CClockDriftMgr();

	// Is clock correction even enabled right now?
	static bool IsClockCorrectionEnabled();

	// Clear our state.
	void Clear();

	// This is called each time a server packet comes in. It is used to correlate
	// where the server is in time compared to us.
	void SetServerTick(int iServerTick);

	// Pass in the frametime you would use, and it will drift it towards the server clock.
	float AdjustFrameTime(float inputFrameTime);

	// Returns how many ticks ahead of the server the client is.
	float GetCurrentClockDifference() const;

private:
	void ShowDebugInfo(float flAdjustment);

	// This scales the offsets so the average produced is equal to the
	// current average + flAmount. This way, as we add corrections,
	// we lower the average accordingly so we don't keep responding
	// as much as we need to after we'd adjusted it a couple times.
	void AdjustAverageDifferenceBy(float flAmountInSeconds);

private:
	enum
	{
		// This controls how much it smoothes out the samples from the server.
		NUM_CLOCKDRIFT_SAMPLES = 16
	};

	// This holds how many ticks the client is ahead each time we get a server tick.
	// We average these together to get our estimate of how far ahead we are.
	float m_ClockOffsets[NUM_CLOCKDRIFT_SAMPLES];
	int   m_iCurClockOffset;

	int m_nServerTick; // Last-received tick from the server.
	int m_nClientTick; // The client's own tick counter (specifically, for interpolation during rendering).
					   // The server may be on a slightly different tick and the client will drift towards it.
};

class CPrecacheItem
{
public:
	CPrecacheItem()
	{
	}

public:
	/*tstring*/ int      m_sClass;
	CBaseHandle m_hEntity;
};

struct AddAngle
{
	float total;
	float starttime;
};

// =========================================================================
// CBaseClientState
// !!! This can only really be used as a place holder as the compiler optimizes many of the internals as static variables !!!
// It is also possible that this has changed since 2007
// =========================================================================
class CBaseClientState : public INetChannelHandler, public IConnectionlessPacketHandler, public IServerMessageHandler
{
};

class ClientClass;
class CPureServerWhitelist;

//-----------------------------------------------------------------------------
// Purpose: CClientState should hold all pieces of the client state
//   The client_state_t structure is wiped completely at every server signon
//-----------------------------------------------------------------------------
class CClientState
{
public:
	INetChannel * GetNetChan()
	{
		return *reinterpret_cast<INetChannel**>(reinterpret_cast<DWORD>(this) + 0x10);
	}

	int GetDeltaTick()
	{
		return *(int*)(this + 0x19C);
	}
};