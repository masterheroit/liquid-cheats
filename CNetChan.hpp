#pragma once

#ifndef __CNETCHAN_HPP
#define __CNETCHAN_HPP

#include <inetchannel.h>
#include <inetchannelinfo.h>
#include <utlvector.h>
#include <public/basehandle.h>
#include <netadr.h>
#include <common/qlimits.h>
#include <filesystem.h>

#define MAX_STREAMS 256
class CNetChan : public INetChannel
{

public: // netchan structurs
	typedef struct dataFragments_s
	{
		FileHandle_t file;                 // open file handle
		char         filename[MAX_OSPATH]; // filename
		char *       buffer;               // if NULL it's a file
		unsigned int BYTEs;                // size in BYTEs
		unsigned int bits;                 // size in bits
		unsigned int transferID;           // only for files
		bool         isCompressed;         // true if data is bzip compressed
		unsigned int nUncompressedSize;    // full size in BYTEs
		bool         asTCP;                // send as TCP stream
		int          numFragments;         // number of total fragments
		int          ackedFragments;       // number of fragments send & acknowledged
		int          pendingFragments;     // number of fragments send, but not acknowledged yet
	} dataFragments_t;

	struct subChannel_s
	{
		int startFraggment[MAX_STREAMS];
		int numFragments[MAX_STREAMS];
		int sendSeqNr;
		int state; // 0 = free, 1 = scheduled to send, 2 = send & waiting, 3 = dirty
		int index; // index in m_SubChannels[]

		void Free()
		{
			state = 0;
			sendSeqNr = -1;
			for (int i = 0; i < MAX_STREAMS; i++) {
				numFragments[i] = 0;
				startFraggment[i] = -1;
			}
		}
	};

	// Client's now store the command they sent to the server and the entire results set of
	//  that command.
	typedef struct netframe_s
	{
		// Data received from server
		float          time;        // net_time received/send
		int            size;        // total size in BYTEs
		float          latency;     // raw ping for this packet, not cleaned. set when acknowledged otherwise -1.
		float          avg_latency; // averaged ping for this packet
		bool           valid;       // false if dropped, lost, flushed
		int            choked;      // number of previously chocked packets
		int            dropped;
		float          m_flInterpolationAmount;
		unsigned short msggroups[INetChannelInfo::TOTAL]; // received BYTEs for each message group
	} netframe_t;

	typedef struct
	{
		float       nextcompute;      // Time when we should recompute k/sec data
		float       avgBYTEspersec;   // average BYTEs/sec
		float       avgpacketspersec; // average packets/sec
		float       avgloss;          // average packet loss [0..1]
		float       avgchoke;         // average packet choke [0..1]
		float       avglatency;       // average ping, not cleaned
		float       latency;          // current ping, more accurate also more jittering
		int         totalpackets;     // total processed packets
		int         totalBYTEs;       // total processed BYTEs
		int         currentindex;     // current frame index
		netframe_t  frames[64];       // frame history
		netframe_t *currentframe;     // current frame
	} netflow_t;

public:
	CNetChan() = delete;

	bool m_bProcessingMessages;
	bool m_bShouldDelete;

	// last send outgoing sequence number
	int m_nOutSequenceNr;
	// last received incoming sequnec number
	int m_nInSequenceNr;
	// last received acknowledge outgoing sequnce number
	int m_nOutSequenceNrAck;

	// state of outgoing reliable data (0/1) flip flop used for loss detection
	int m_nOutReliableState;
	// state of incoming reliable data
	int m_nInReliableState;

	int m_nChokedPackets; // number of choked packets

						  // Reliable data buffer, send which each packet (or put in waiting list)
	bf_write         m_StreamReliable;
	CUtlMemory<BYTE> m_ReliableDataBuffer;

	// unreliable message buffer, cleared which each packet
	bf_write         m_StreamUnreliable;
	CUtlMemory<BYTE> m_UnreliableDataBuffer;

	bf_write         m_StreamVoice;
	CUtlMemory<BYTE> m_VoiceDataBuffer;

	// don't use any vars below this (only in net_ws.cpp)

	int m_Socket;       // NS_SERVER or NS_CLIENT index, depending on channel.
	int m_StreamSocket; // TCP socket handle

	unsigned int m_MaxReliablePayloadSize; // max size of reliable payload in a single packet

										   // Address this channel is talking to.
	netadr_t remote_address;

	// For timeouts.  Time last message was received.
	float last_received;
	// Time when channel was connected.
	double connect_time;

	// Bandwidth choke
	// BYTEs per second
	int m_Rate;
	// If realtime > cleartime, free to send next packet
	double m_fClearTime;

	CUtlVector<dataFragments_t *> m_WaitingList[MAX_STREAMS]; // waiting list for reliable data and file transfer
	dataFragments_t               m_ReceiveList[MAX_STREAMS]; // receive buffers for streams
	subChannel_s                  m_SubChannels[8];

	unsigned int m_FileRequestCounter;         // increasing counter with each file request
	bool         m_bFileBackgroundTranmission; // if true, only send 1 fragment per packet
	bool         m_bUseCompression;            // if true, larger reliable data will be bzip compressed

											   // TCP stream state maschine:
	bool             m_StreamActive;          // true if TCP is active
	int              m_SteamType;             // STREAM_CMD_*
	int              m_StreamSeqNr;           // each blob send of TCP as an increasing ID
	int              m_StreamLength;          // total length of current stream blob
	int              m_StreamReceived;        // length of already received BYTEs
	char             m_SteamFile[MAX_OSPATH]; // if receiving file, this is it's name
	CUtlMemory<BYTE> m_StreamData;            // Here goes the stream data (if not file). Only allocated if we're going to use it.

											  // packet history
	netflow_t m_DataFlow[MAX_FLOWS];
	int       m_MsgStats[INetChannelInfo::TOTAL]; // total BYTEs for each message group

	int m_PacketDrop; // packets lost before getting last update (was global net_drop)

	char m_Name[32]; // channel name

	unsigned int m_ChallengeNr; // unique, random challenge number

	float m_Timeout; // in seconds

	INetChannelHandler *      m_MessageHandler; // who registers and processes messages
	CUtlVector<INetMessage *> m_NetMessages;    // list of registered message
	IDemoRecorder *           m_DemoRecorder;   // if != NULL points to a recording/playback demo object
	int                       m_nQueuedPackets;

	float m_flInterpolationAmount;
	float m_flRemoteFrameTime;
	float m_flRemoteFrameTimeStdDeviation;
	int   m_nMaxRoutablePayloadSize;

	int m_nSplitPacketSequence;
};

#endif