#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <d3dx9.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#ifndef __authapi
#define __authapi

class C_AuthAPI {
public:
	void thread(LPVOID args);

	//
	//Inline Functions;
	//
	
	inline bool wsa_init() {
		WSADATA wsaData;

		return (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
	}

	//
	//
	//

	bool m_Connected;
	bool m_AuthSucceeded;
};

#endif